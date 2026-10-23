/*
 * A122537  a(1) = 1; for n>1, a(n) is smallest number greater than a(n-1),
 *          divisible by n and not equal to any a(i)+a(j) with i and j <= n-1.
 * https://oeis.org/A122537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A122537
   a(1) = 1;
   for n > 1, a(n) is the smallest integer > a(n‑1) that is
   divisible by n and that is not equal to any a(i)+a(j) with
   1 ≤ i,j < n
*/
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Maximum number of terms we support (adjust if larger n is needed) */
#define MAX_TERMS 10001

/* Static cache of computed terms.  Index 1 corresponds to a(1) */
static seq_t seq_cache[MAX_TERMS];
/* Indicates which entries are already computed */
static bool  cache_filled[MAX_TERMS] = {false};
static size_t cache_max = 0;          /* largest n for which a(n) is known */

/* Bit‑set storing all forbidden sums a(i)+a(j).  A 1‑bit means the sum
   is forbidden.  The bit position equals the integer value of the sum.
   Dynamic growth as needed. */
static uint64_t *sum_bitmap = NULL;   /* NULL means empty */
static uint64_t bitmap_bits = 0;      /* number of bits currently represented */

/* Ensure that bitmap can represent at least 'need_bit' bits.
   If it needs to grow, increase by factor of two. */
static inline void ensure_bitmap_bits(uint64_t need_bit)
{
    if (need_bit < bitmap_bits) return;
    uint64_t new_bits = bitmap_bits ? bitmap_bits << 1 : 64;
    while (new_bits <= need_bit) new_bits <<= 1;
    /* Reallocate to new size */
    uint64_t *tmp = (uint64_t *)realloc(sum_bitmap, (new_bits >> 6) * sizeof(uint64_t));
    if (!tmp) {               /* allocation failure – abort */
        abort();
    }
    /* Zero the newly added words */
    for (uint64_t i = bitmap_bits >> 6; i < new_bits >> 6; ++i)
        tmp[i] = 0;
    sum_bitmap = tmp;
    bitmap_bits = new_bits;
}

/* Set the bit for the given sum value */
static inline void set_sum_bit(uint64_t val)
{
    ensure_bitmap_bits(val);
    sum_bitmap[val >> 6] |= 1ULL << (val & 63);
}

/* Test whether a bit is set for the given sum value */
static inline bool sum_bit_set(uint64_t val)
{
    if (val >= bitmap_bits) return false;
    return (sum_bitmap[val >> 6] >> (val & 63)) & 1ULL;
}

/* Compute all terms up to n (including n) and store them in seq_cache.
   The function is safe to call in any order of n. */
static void compute_up_to(idx_t n)
{
    if (n <= cache_max) return;           /* already computed */

    /* Handle base case a(1)=1 */
    if (cache_max == 0) {
        seq_cache[1] = (seq_t)1;
        cache_filled[1] = true;
        /* Sum a(1)+a(1) must be forbidden */
        set_sum_bit((uint64_t)1 + 1);
        cache_max = 1;
    }

    /* Compute subsequent terms */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        /* Find the smallest multiple of k that is > a(k-1) */
        seq_t prev = seq_cache[k - 1];
        seq_t rem  = prev % (seq_t)k;
        seq_t candidate = (rem == 0) ? prev + (seq_t)k
                                     : prev + ((seq_t)k - rem);

        /* Ensure the candidate is not a forbidden sum */
        while (true) {
            uint64_t cand = (uint64_t)candidate;   /* fits in 64 bits for n ≤ 10000 */
            if (!sum_bit_set(cand)) break;
            candidate += (seq_t)k;
        }

        /* Store the new term */
        seq_cache[k] = candidate;
        cache_filled[k] = true;

        /* Record all new forbidden sums involving the new term */
        for (idx_t i = 1; i <= k; ++i) {
            seq_t sum = candidate + seq_cache[i];
            set_sum_bit((uint64_t)sum);
        }
    }
    cache_max = n;
}

/* Public function as required */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* out of bounds – return 0 */
    if (n > MAX_TERMS - 1)        /* ensure we stay inside the static array */
        return (seq_t)0;
    compute_up_to(n);
    return seq_cache[n];
}
