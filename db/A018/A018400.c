/*
 * A018400  Divisors of 338.
 * https://oeis.org/A018400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018400: divisors of 338 in increasing order. The
   function generates the divisor list once and returns the
   n‑th (1‑based) element. */
#include <stdbool.h>
#include <stddef.h>

static seq_t divs[6];
static bool built = false;

static void build_divisors(void)
{
    if (built) return;
    built = true;
    /* 338 = 2^1 * 13^2, generate all combinations of exponents
       e1 ∈ {0,1}, e2 ∈ {0,1,2}. */
    size_t k = 0;
    seq_t pow1;
    for (unsigned int i = 0; i <= 1; ++i) {
        pow1 = 1;
        for (unsigned int a = 0; a < i; ++a)
            pow1 *= (seq_t)2;
        seq_t pow2;
        for (unsigned int j = 0; j <= 2; ++j) {
            pow2 = 1;
            for (unsigned int b = 0; b < j; ++b)
                pow2 *= (seq_t)13;
            divs[k++] = pow1 * pow2;
        }
    }
    /* sort in increasing order (in‑place) */
    for (size_t i = 0; i < 6; ++i) {
        for (size_t j = i + 1; j < 6; ++j) {
            if (divs[i] > divs[j]) {
                seq_t t = divs[i];
                divs[i] = divs[j];
                divs[j] = t;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset below defined */
    build_divisors();
    if (n <= (idx_t)6)
        return divs[(size_t)n - 1];
    return 0;                      /* undefined for n > 6 */
}
