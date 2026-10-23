/*
 * A137992  A014137 (= partial sums of Catalan numbers A000108) mod 3.
 * https://oeis.org/A137992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum_{k=0..n} Catalan(k) (mod 3)
 * where Catalan(k) = binomial(2k,k)/(k+1) and the sum is taken modulo 3.
 */
#include <stdlib.h>

static seq_t *catalan = NULL;   /* Catalan numbers modulo 3 */
static seq_t *partial = NULL;   /* cumulative sums modulo 3 */
static idx_t  max_computed = 0; /* largest n for which we have computed */

seq_t a(idx_t n)
{
    /* initialise on first call */
    if (catalan == NULL) {
        catalan = (seq_t*)malloc(sizeof(seq_t));
        partial = (seq_t*)malloc(sizeof(seq_t));
        if (!catalan || !partial) abort();      /* allocation failure */
        catalan[0] = 1 % 3;                      /* C(0) = 1 */
        partial[0] = 1 % 3;                      /* a(0) = 1 */
        max_computed = 0;
    }

    /* extend tables only if needed */
    if (n > max_computed) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(catalan, new_len * sizeof(seq_t));
        seq_t *tmp2 = (seq_t*)realloc(partial, new_len * sizeof(seq_t));
        if (!tmp || !tmp2) abort();
        catalan = tmp;
        partial = tmp2;

        for (idx_t k = max_computed + 1; k <= n; ++k) {
            /* compute Catalan(k) via convolution: C(k) = Σ_{i=0}^{k-1} C(i)*C(k-1-i) */
            seq_t sum = 0;
            idx_t prev = k - 1;
            for (idx_t i = 0; i <= prev; ++i) {
                seq_t prod = (catalan[i] * catalan[prev - i]) % 3;
                sum += prod;
                if (sum >= 3) sum -= 3;        /* keep modulo 3 */
            }
            catalan[k] = sum;

            /* cumulative sum modulo 3 */
            seq_t p = partial[k - 1] + sum;
            if (p >= 3) p -= 3;
            partial[k] = p;
        }
        max_computed = n;
    }

    return partial[n];
}
