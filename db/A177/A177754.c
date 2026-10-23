/*
 * A177754  Partial sums of A047994.
 * https://oeis.org/A177754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} ∏_{p^e‖i} (p^e - 1)
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*  Prime sieve (small‑prime–factor array) and partial sums are cached
    across calls.  The arrays are grown only when a larger n is requested.
*/
static seq_t *partial = NULL;   /* partial sums a[0..N]   */
static size_t partial_len = 0;  /* current maximum computed index  */
static uint32_t *spf = NULL;    /* smallest prime factor for each number   */
static size_t spf_len = 0;      /* current size of spf array               */

static void recompute_upto(idx_t n)
{
    size_t i, j;
    size_t new_len = (size_t)n + 1;

    /* rebuild smallest‑prime‑factor array up to n */
    uint32_t *new_spf = (uint32_t *)malloc(new_len * sizeof(uint32_t));
    for (i = 0; i < new_len; ++i) new_spf[i] = 0;
    if (new_len > 1) new_spf[1] = 1;
    for (i = 2; i < new_len; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if (i * i < new_len) {
                for (j = i * i; j < new_len; j += i)
                    if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
            }
        }
    }
    if (spf) free(spf);
    spf      = new_spf;
    spf_len  = new_len;

    /* rebuild partial sums */
    seq_t *new_partial = (seq_t *)malloc(new_len * sizeof(seq_t));
    new_partial[0] = 0;
    for (i = 1; i < new_len; ++i) {
        seq_t prod = 1;
        idx_t x = (idx_t)i;
        if (i == 1) {
            new_partial[i] = 1;
            continue;
        }
        while (x > 1) {
            idx_t p = (idx_t)spf[x];
            seq_t pow = 1;
            while (x % p == 0) {
                x /= p;
                pow *= (seq_t)p;
            }
            prod *= (pow - 1);
        }
        new_partial[i] = new_partial[i - 1] + prod;
    }
    if (partial) free(partial);
    partial      = new_partial;
    partial_len  = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if ((size_t)n >= partial_len) recompute_upto(n);
    return partial[n];
}
