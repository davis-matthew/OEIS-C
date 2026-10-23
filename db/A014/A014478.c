/*
 * A014478  Inverse of 469th cyclotomic polynomial.
 * https://oeis.org/A014478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   Let φ(x) = Φ_469(x) = Σ_{i=0}^{396} φ_i x^i.
   The sequence a(n) is defined by a(0)=1 and
   for n>0: Σ_{i=0}^{min(n,396)} φ_i · a(n−i) = 0.
   Thus a(n) = −Σ_{i=1}^{min(n,396)} φ_i · a(n−i).
   The sequence is periodic with period 469. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* The build supplies seq_t and idx_t definitions. This header is
   only needed to obtain size_t. */

/* Forward declaration of helper. */
static void init_seq(void);

/* Storage for the periodic values a(0)…a(468). */
static seq_t inv[469];

/* Storage for the coefficients φ_i of Φ_469(x), i=0…396. */
static seq_t phi[397];

#define NDIG 470   /* degree of numerator x^470−x^469−x+1   */
#define DDIG 74    /* degree of denominator x^74−x^67−x^7+1 */

static void init_seq(void)
{
    static bool built = false;
    if (built)
        return;
    built = true;

    /* Allocate temporary arrays for the polynomial division.  */
    seq_t N[NDIG + 1];
    seq_t D[DDIG + 1];
    seq_t T[NDIG + 1];

    /* Zero initialise temporary arrays. */
    for (size_t i = 0; i <= NDIG; ++i)
        N[i] = 0;
    for (size_t i = 0; i <= DDIG; ++i)
        D[i] = 0;

    /* Construct numerator: (x^469 - 1)·(x - 1) = x^470 - x^469 - x + 1 */
    N[NDIG]     = 1;   /* x^470          */
    N[NDIG - 1] = -1;  /* -x^469         */
    N[1]        = -1;  /* -x             */
    N[0]        = 1;   /* +1             */

    /* Construct denominator: (x^67 - 1)(x^7 - 1) = x^74 - x^67 - x^7 + 1 */
    D[DDIG] = 1;   /* x^74   */
    D[67]   = -1;  /* -x^67  */
    D[7]    = -1;  /* -x^7   */
    D[0]    = 1;   /* +1     */

    /* Copy N into T for the division. */
    for (size_t i = 0; i <= NDIG; ++i)
        T[i] = N[i];

    /* Perform long division T(x) / D(x) to obtain Φ_469(x). */
    for (int i = NDIG; i >= DDIG; --i)
    {
        seq_t q = T[i];                /* leading coefficient of the current dividend part */
        size_t shift = (size_t)(i - DDIG);
        phi[shift] = q;                /* φ_{shift} is the quotient coefficient */
        for (size_t j = 0; j <= DDIG; ++j)
        {
            T[shift + j] -= q * D[j];
        }
    }
    /* The constant term φ[0] is already set in the loop when shift == 0. */

    /* Compute the first 469 terms of the reciprocal series. */
    inv[0] = 1;
    for (size_t k = 1; k < 469; ++k)
    {
        seq_t sum = 0;
        size_t max_i = k < 397 ? k : (size_t)396;
        for (size_t i = 1; i <= max_i; ++i)
        {
            sum += phi[i] * inv[k - i];
        }
        inv[k] = -sum;   /* because φ[0] = 1 */
    }
}

seq_t a(idx_t n)
{
    init_seq();
    return inv[(size_t)(n % 469)];
}
