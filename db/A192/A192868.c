/*
 * A192868  Thin numbers: odd numbers k > 1 such that k+1 is a prime times a
 *          power of two.
 * https://oeis.org/A192868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 7488, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Thin numbers: odd k > 1 such that k+1 = prime × 2^m (m ≥ 1) */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime_big.h"

/* Static cache of thin numbers.
   thin_cache holds the found thin numbers,
   thin_len is the number stored,
   thin_cap is the allocated capacity,
   thin_next is the next odd number to be examined. */
static seq_t *thin_cache = NULL;
static size_t thin_cap   = 0;
static size_t thin_len   = 0;
static idx_t  thin_next  = 3;

/* Test whether an odd number k > 1 is thin.
   k+1 must be of the form prime × 2^m.  The odd part is
   extracted by right‑shifting until it becomes odd.  If this
   odd part is 1, *k+1* is a power of two, hence thin because the
   prime factor is 2.  Otherwise the odd part must be prime. */
static bool isThin(seq_t k)
{
    seq_t x = k + 1;
    while ((x & 1) == 0)          /* strip all factors of two */
        x >>= 1;
    if (x == 1)                   /* k+1 is a power of two -> thin */
        return true;
    return is_prime_big(x);       /* remaining odd part must be prime */
}

/* Make sure at least 'target' thin numbers have been cached. */
static void ensure_thin(idx_t target)
{
    if (thin_len >= target)
        return;

    for (idx_t k = thin_next; ; k += 2) {
        if (isThin((seq_t)k)) {
            if (thin_len == thin_cap) {
                size_t new_cap = thin_cap ? thin_cap << 1 : 16;
                grow_seq(&thin_cache, &thin_cap, new_cap);
            }
            thin_cache[thin_len++] = (seq_t)k;
            if (thin_len == target) {
                thin_next = k + 2;   /* next candidate after this one */
                return;
            }
        }
    }
}

/* Public function required by the problem statement.
   a(1) is the first thin number, a(2) the second, and so on.
   For n == 0 this returns 0 (unused by the OEIS file). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n < 1, harmless */
    ensure_thin(n);
    return thin_cache[n - 1];
}
