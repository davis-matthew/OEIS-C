/*
 * A200513  Least m>0 such that n = y^2 - 3^x (mod m) has no solution, or 0 if
 *          no such m exists.
 * https://oeis.org/A200513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A200513
   a(n) = least m>0 such that the congruence y² – 3ˣ ≡ n (mod m) has no solution,
          or 0 if n is representable exactly as y² – 3ˣ.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/floor_sqrt.h"

#define MAXM 20001          /* search limit for modulus m */

static bool exact_representable(idx_t n)
{
    /* check whether there exist integers x,y with y² – 3ˣ = n */
    seq_t pow3 = 1;                  /* 3^x */
    const int XMAX = 20;             /* sufficient for n up to 10⁴ */
    for (int x = 0; x < XMAX; ++x) {
        seq_t T = pow3 + (seq_t)n;   /* y² = n + 3ˣ     */
        seq_t s = floor_sqrt(T);
        if (s * s == T) return true;
        pow3 *= 3;
    }
    return false;
}

static char sq_arr[MAXM];
static char vis_arr[MAXM];

seq_t a(idx_t n)
{
    if (exact_representable(n))
        return 0;

    idx_t modn = n; /* will be reduced modulo m inside loop */

    for (idx_t m = 1; m < MAXM; ++m) {
        /* reset arrays for current modulus m */
        memset(sq_arr, 0, (size_t)m);
        memset(vis_arr, 0, (size_t)m);

        /* pre‑compute quadratic residues modulo m */
        for (idx_t i = 0; i < m; ++i) {
            seq_t v = ((seq_t)i * (seq_t)i) % m;
            sq_arr[(size_t)v] = 1;
        }

        /* iterate over distinct powers of 3 modulo m */
        bool found = false;
        idx_t r = 1 % m;              /* 3⁰ mod m */
        idx_t nmod = (idx_t)(n % m);
        while (!vis_arr[(size_t)r]) {
            vis_arr[(size_t)r] = 1;
            idx_t target = (nmod + r) % m;
            if (sq_arr[(size_t)target]) {
                found = true;
                break;
            }
            r = (r * 3) % m;
        }

        if (!found) {
            return (seq_t)m;
        }
    }

    /* If no modulus m up to MAXM worked, assume n is representable */
    return 0;
}
