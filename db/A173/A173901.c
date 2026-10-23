/*
 * A173901  Numbers n with property that the square of each digit of n is a
 *          substring of n^3.
 * https://oeis.org/A173901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = nth integer n such that for each decimal digit d of n, *
 *            d^2 as a string occurs as a contiguous substring of n³
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 134, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, digit_len, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/big_pow.h"
#include "../utils/seq_to_str.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* decimal squares of 0‑9 as strings */
    static const char *sqstr[10] = {
        "0", "1", "4", "9", "16",
        "25", "36", "49", "64", "81"
    };

    seq_t count = 0;  /* how many good numbers found so far */
    seq_t cand  = 0;   /* candidate number */

    for (;;) {
        cand = cand + 1;                       /* test next integer */

        seq_t cube = big_pow(cand, 3);         /* cand³ */

        /* convert cube to a null‑terminated decimal string */
        size_t cube_digits = digit_len(cube);
        char *cube_str = (char *)malloc(cube_digits + 1);
        if (!cube_str) {
            return (seq_t)0;                   /* out of memory: abort */
        }
        size_t cube_len;
        seq_to_str(cube, cube_str, &cube_len);
        cube_str[cube_len] = '\0';

        /* test whether every digit of cand has its square present */
        bool ok = true;
        seq_t t = cand;
        while (ok && t != 0) {
            unsigned d = (unsigned)(t % 10);   /* last decimal digit */
            if (strstr(cube_str, sqstr[d]) == NULL) {
                ok = false;
            }
            t = t / 10;
        }
        /* cand == 0 is never tested; we start at 1 */

        free(cube_str);

        if (ok) {
            count = count + 1;
            if (count == n) {
                return cand;                   /* nth term found */
            }
        }
    }
}
