/*
 * A070202  Number of integer triangles with perimeter n, integer inradius and
 *          side lengths that are not relatively prime.
 * https://oeis.org/A070202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute the number of integer triangles of perimeter n that have a
 * positive integer inradius and whose side lengths are not relatively
 * prime.  The algorithm enumerates all triples (a,b,c) with
 *   1 ≤ a ≤ b ≤ c,
 *   a + b + c = n,
 *   a + b > c   (triangle inequality).
 * For each triple it tests
 *   gcd(a,b,c) > 1     (the sides are not coprime)
 *   (s – a)(s – b)(s – c) / s is a perfect square,
 *   where s = n/2 is the semiperimeter.  This second test is the
 *   integer‑inradius condition derived from Heron’s formula.
 *
 * The search uses 64‑bit integer arithmetic, performed in the
 * seq_t type; the code works for any seq_t that supports the
 * required operators.  Inclusion of <stdbool.h> is required for
 * the bool type used by floor_sqrt().
 */
#include "../utils/gcd.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* values for perimeters less than 3 cannot form a triangle */
    if (n < 3)
        return 0;

    /* perimeter must be even for a triangle with integer inradius;
       this check is optional but helps skip invalid n quickly. */
    if (n & 1)
        return 0;

    const seq_t count_initial = 0;
    seq_t count = count_initial;

    const seq_t half = (seq_t)n / 2;            /* semiperimeter s */
    const idx_t max_a = n / 3;                  /* a ≤ n/3 to keep a ≤ b ≤ c */

    for (idx_t a_val = 1; a_val <= max_a; ++a_val) {
        /* triangle inequality: a + b > s  →  b > s – a */
        idx_t b_min = a_val;
        idx_t temp = (idx_t)half - a_val;
        if ((idx_t)temp + 1 > b_min)
            b_min = (idx_t)temp + 1;

        /* b ≤ c  →  b ≤ (n – a)/2 */
        idx_t b_max = (n - a_val) / 2;
        if (b_min > b_max)
            continue;

        for (idx_t b_val = b_min; b_val <= b_max; ++b_val) {
            idx_t c_val = n - a_val - b_val;
            if (c_val < b_val)      /* safety; should not happen */
                continue;

            /* sides must not be coprime: gcd(a,b,c) > 1 */
            seq_t g = gcd((seq_t)a_val, (seq_t)b_val);
            g = gcd(g, (seq_t)c_val);
            if (g <= (seq_t)1)
                continue;

            /* test for integer inradius using Heron’s formula */
            seq_t u = (seq_t)half - (seq_t)a_val;
            seq_t v = (seq_t)half - (seq_t)b_val;
            seq_t w = (seq_t)half - (seq_t)c_val;

            /* (s – a)(s – b)(s – c) must be divisible by s */
            seq_t t = u * v * w;
            if (t % half != 0)
                continue;

            seq_t q = t / half;           /* q = (s–a)(s–b)(s–c)/s */
            seq_t r = floor_sqrt(q);      /* floor(sqrt(q)) */
            if (r * r == q)
                ++count;
        }
    }

    return count;
}
