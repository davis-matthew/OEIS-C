/*
 * A015082  Inverse of 1073rd cyclotomic polynomial.
 * https://oeis.org/A015082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inverse of 1073rd cyclotomic polynomial.
   The coefficients a(n) of the power series
   1 / Φ_{1073}(x) are periodic with period 1073.
   Let b[0..65] be the coefficients of
          B(x) = Φ₁(x)·Φ₂₉(x)·Φ₃₇(x)
       = (x²⁹–1)·(x³⁷–1)/(x–1).
   Then for any n ≥ 0
          a(n) = –b[n mod 1073]   if (n mod 1073) ≤ 65
                 0                 otherwise.
   This implementation computes b[] once and
   uses the above formula to return a(n). */

#include <stdbool.h>

static void compute_b(seq_t *b)
{
    /* P(x) = (x^66 – x^37 – x^29 + 1)   coefficients a[0..66] */
    seq_t a[67];
    for (int i = 0; i < 67; ++i)
        a[i] = (seq_t)0;
    a[0] = (seq_t)1;
    a[29] = (seq_t)-1;
    a[37] = (seq_t)-1;
    a[66] = (seq_t)1;

    /* Divide P(x) by (x – 1) to get b(x) = Q(x) of degree 65.
       Synthetic division with root 1: q[65]=a[66];
       for i=65 downto 1: q[i-1] = a[i] + q[i];
       Finally q[0] satisfies -q[0] = a[0] (checked implicitly). */
    b[65] = a[66];
    for (int i = 65; i >= 1; --i)
        b[i - 1] = a[i] + b[i];
}

seq_t a(idx_t n)
{
    /* Compute the pattern coefficients once */
    static bool initialized = false;
    static seq_t b[66];
    if (!initialized) {
        compute_b(b);
        initialized = true;
    }

    const idx_t period = 1073ULL;      /* 29 × 37 */
    idx_t r = n % period;              /* n mod 1073 */

    if (r <= 65)          /* 0 .. 65 are non‑zero positions */
        return -(b[r]);
    else
        return (seq_t)0;
}
