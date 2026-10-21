/*
 * A209258  The original Josephus problem: 41 soldiers are arranged in a ring,
 *          and every third man is killed by his neighbor, until only the last
 *          person remains, who would kill himself. Sequence shows soldier
 *          killing order.
 * https://oeis.org/A209258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Josephus elimination order for 41 soldiers, step size 3 */
/* a(n) is the nth killed soldier (a(1) = 3, a(2) = 6, …, a(41) = 7) */

#include <stddef.h>     /* for size_t */
#include <stdbool.h>    /* for bool */

#define N 41            /* number of soldiers */
#define K 3             /* counting step */

seq_t a(idx_t n)
{
    /* Static cache of the killing order (computed once) */
    static seq_t kills[N];
    static bool initialized = false;

    if (!initialized) {
        /* Array of currently alive soldiers (values 1..N) */
        idx_t alive[N];
        for (idx_t i = 0; i < N; ++i) {
            alive[i] = i + 1;
        }

        idx_t alive_cnt = N;
        idx_t pos = 0;            /* start from soldier 1 */

        for (idx_t i = 0; i < N; ++i) {
            /* Find the next victim */
            pos = (pos + K - 1) % alive_cnt;
            kills[i] = (seq_t)alive[pos];

            /* Remove the victim from the circle */
            for (idx_t j = pos; j + 1 < alive_cnt; ++j) {
                alive[j] = alive[j + 1];
            }
            --alive_cnt;
        }

        initialized = true;
    }

    /* Undefined indices return 0, as required for indices below the offset */
    if (n == 0 || n > N)
        return (seq_t)0;
    return kills[n - 1];
}
