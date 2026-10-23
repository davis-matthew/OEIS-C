/*
 * A161663  Number of reduced words of length n in the Weyl group A_42.
 * https://oeis.org/A161663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in the polynomial ∏_{k=1}^{42} (1 + x
 *            + … + x^k). This is also the number of reduced words of length n
 *            in the Weyl group A_42. The generating function is G(x) =
 *            ∏_{k=1}^{42} (1 - x^(k+1)) / (1 - x), which expands to a
 *            polynomial of degree 903 (1+2+…+42) and all coefficie...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build system.
   For this routine we treat seq_t as an unsigned integer type
   (possibly arbitrary precision).  */

static seq_t cache[904] = {0};
static bool computed = false;

/* Compute the whole table of coefficients once.  */
static void
compute_cache(void)
{
    /* zero all entries – the static initializer already does this,
       but keep it explicit for clarity.  */
    for (idx_t i = 0; i < 904; ++i)
        cache[i] = 0;

    cache[0] = 1;  /* product of zero factors is 1 */

    idx_t current_max = 0;          /* maximum index with non‑zero value so far */
    for (idx_t k = 1; k <= 42; ++k) {
        /* For each k, multiply the current polynomial by (1 + x + … + x^k).
           Using descending indices keeps the DP in place.  */
        for (idx_t sum = current_max; sum != (idx_t)-1; --sum) {
            seq_t base = cache[sum];
            for (idx_t i = 1; i <= k; ++i) {
                cache[sum + i] += base;
            }
        }
        current_max += k;   /* the degree grows by k each step */
    }

    computed = true;
}

/* Main function requested by the specification.  */
seq_t
a(idx_t n)
{
    if (!computed) {
        compute_cache();
    }

    if (n >= 904) {
        return 0;          /* terms beyond the degree are zero */
    }
    return cache[n];
}
