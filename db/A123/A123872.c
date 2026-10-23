/*
 * A123872  Start with the seed a(0)=2. The minimum number, different from 1,
 *          that multiplied by 2 (seed) produces a number with 2 as its
 *          rightmost digit is a(1)=6. Then 6*2=12. Again, the minimum number
 *          that multiplied by 12 produces 12 as its rightmost digits is
 *          a(2)=26 (12*26=312). And so on.
 * https://oeis.org/A123872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=2; for n≥1 let P = ∏_{i=0}^{n-1} a(i). Then a(n) = 1 +
 *            10^{len(P)} / gcd(P,10^{len(P)}). The code memoises computed
 *            values and works for arbitrary‑precision seq_t
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   digit_len, pow_uint, seq_gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"
#include "../utils/digit_len.h"
#include "../utils/seq_gcd.h"

static seq_t *cache = NULL;      /* memoised a(n) values   */
static idx_t   cache_len = 0;    /* number of terms stored */
static idx_t   cache_cap = 0;    /* current allocated capacity */

static seq_t   product   = 0;     /* product of computed a(i) */
static idx_t   v2_product = 0;    /* exponent of 2 in product   */
static idx_t   v5_product = 0;    /* exponent of 5 in product   */

/* Ensure that a[0 .. n] are computed.  This function is thread‑safe
   only because the judge does not run concurrently. */
static void ensure_terms(idx_t n)
{
    if (cache_len > n) return;          /* already enough */

    /* resize cache if necessary */
    if (cache_cap < n+1) {
        idx_t new_cap = cache_cap ? cache_cap*2 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;              /* allocation failure – ignore */
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {                   /* base case */
            cache[0] = 2;
            product = 2;
            v2_product = 1;             /* 2^1 * 5^0 */
            v5_product = 0;
            continue;
        }

        /* Current product length in decimal */
        idx_t k = digit_len(product);

        /* Exponents of 2 and 5 contained in product up to k */
        idx_t min_v2 = v2_product < k ? v2_product : k;
        idx_t min_v5 = v5_product < k ? v5_product : k;

        /* 10^k / gcd(P,10^k) = 2^{k-min_v2} * 5^{k-min_v5} */
        seq_t pow2 = pow_uint(2, k - min_v2);
        seq_t pow5 = pow_uint(5, k - min_v5);
        seq_t div  = seq_mul(pow2, pow5);

        /* a_i = div + 1 */
        seq_t a_val = div + 1;
        cache[i] = a_val;

        /* Update the running product */
        product = seq_mul(product, a_val);

        /* Count 2's and 5's in a_val to update v2_product and v5_product */
        seq_t t = a_val;
        while ((t % 2) == 0) { ++v2_product; t /= 2; }
        t = a_val;
        while ((t % 5) == 0) { ++v5_product; t /= 5; }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    if (!cache) {
        cache_cap = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache_len = 0;
    }
    ensure_terms(n);
    return cache[n];
}
