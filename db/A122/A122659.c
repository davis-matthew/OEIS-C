/*
 * A122659  Cubes whose digits occur exactly twice.
 * https://oeis.org/A122659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: cubes whose digits occur exactly twice (search). */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined for n >= 1. For n == 0 we return 0. */
        return (seq_t)0;
    }

    idx_t k = 1;          /* integer whose cube we test */
    idx_t count = 0;      /* number of cubes found so far */

    for (;; ++k) {
        /* Compute k^3 */
        seq_t cube = (seq_t)k * k * k;

        /* Count digit frequencies of cube */
        unsigned char cnt[10] = {0};
        seq_t tmp = cube;
        bool ok = true;

        if (tmp == 0) {            /* handle the case k = 0 (won't happen),
                                      but makes the code safe */
            cnt[0] = 1;
            ok = false;
        }
        while (tmp > 0) {
            int d = (int)(tmp % 10);
            tmp /= 10;
            unsigned char newcnt = ++cnt[d];
            if (newcnt > 2) {      /* more than twice – reject */
                ok = false;
                break;
            }
        }

        if (ok) {
            /* Reject if any digit occurs exactly once */
            for (int d = 0; d < 10; ++d) {
                if (cnt[d] == 1) {
                    ok = false;
                    break;
                }
            }
        }

        if (ok) {
            ++count;
            if (count == n) {
                return cube;
            }
        }
    }
}
