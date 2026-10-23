/*
 * A125642  Divide the circle into ten "decants" (each of 36 degrees). Let z =
 *          1/2 + i*14.134725142... be the first nontrivial zero of the
 *          Riemann zeta function. Then a(n) is the decant containing the
 *          argument of 1/n^z.
 * https://oeis.org/A125642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, sign
 * Formula:   not stated
 * Range:     i8 n <= 50, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A125642: a(n) is the decant (10 sectors of 36°
   degrees) containing the argument of 1/n^z where z = 1/2 + i*14.134725142.
   Angle = -Im(z) * ln(n) modulo 2π, mapped to the range (-π,π],
   then converted to degrees and assigned to the appropriate
   decant number. */

#include <math.h>

seq_t a(idx_t n)
{
    /* z = 1/2 + i * t   with t = 14.134725142 */
    const long double t   = 14.134725142L;
    const long double PI  = 3.14159265358979323846264338327950288L;
    const long double TWO_PI = 2.0L * PI;

    /* natural logarithm of n */
    long double ln_n = log((long double)n);

    /* compute the principal argument (in radians) */
    long double phi = -t * ln_n;                        /* raw argument */
    phi = fmodl(phi, TWO_PI);                           /* bring to [-2π,2π) */
    if (phi <= -PI) phi += TWO_PI;                      /* to (-π,π] */
    if (phi > PI)  phi -= TWO_PI;

    /* convert to degrees */
    long double deg = phi * 180.0L / PI;

    int decant;
    if (deg >= 0.0L) {
        /* positive sector: 0° → 36° → 72° → … → 144° → 180° */
        decant = (int)(deg / 36.0L) + 1;
    } else {
        /* negative sector: –36° → –72° → –108° → –144° → –180° */
        long double d = -deg;
        decant = -((int)(d / 36.0L) + 1);
    }

    return (seq_t)decant;
}
