/*
 * A156113  Seven-digit primes that begin with 1 followed by permutations of
 *          digits 2 to 7.
 * https://oeis.org/A156113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the n-th 7‑digit prime that begins with 1 and
   has the remaining digits a permutation of 2,3,4,5,6,7.
   The sequence is finite; a(n) = 0 for n > 63. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Static array to hold all prime candidates (max 6! = 720).  */
static seq_t primes_arr[720];
static size_t prime_cnt = 0;
static bool initialized = false;

/* Comparator for qsort. */
static int cmp(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va > vb) - (va < vb);
}

/* Recursive generation of permutations of digits 2–7. */
static void permute(int depth, uint8_t used_mask, seq_t cur)
{
    if (depth == 6) {
        if (is_prime_big(cur))
            primes_arr[prime_cnt++] = cur;
        return;
    }
    static const uint8_t digits[6] = {2, 3, 4, 5, 6, 7};
    for (int i = 0; i < 6; ++i) {
        if (!(used_mask & (1 << i))) {
            permute(depth + 1, used_mask | (1 << i), cur * 10 + digits[i]);
        }
    }
}

/* Initialises the prime list on first use. */
static void init_primes(void)
{
    if (initialized) return;
    prime_cnt = 0;
    permute(0, 0, 1);               /* start with the leading '1' */
    qsort(primes_arr, prime_cnt, sizeof(seq_t), cmp);
    initialized = true;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    init_primes();
    if (n == 0) return 0;          /* offset is 1 */
    idx_t idx = n - 1;
    if (idx < prime_cnt) return primes_arr[idx];
    return 0;                      /* finite sequence */
}
