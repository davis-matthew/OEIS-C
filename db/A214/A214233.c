/*
 * A214233  Number of 0..7 colorings of an nX4 array circular in the 4
 *          direction with new values 0..7 introduced in row major order.
 * https://oeis.org/A214233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical linear recurrence of order 11 for A214233, a(n) =
 *            1953*a(n-1) -829512*a(n-2) +119749880*a(n-3) -7217451834*a(n-4)
 *            +211005732306*a(n-5) -3163113776744*a(n-6)
 *            +24791539817472*a(n-7) -99982144638429*a(n-8)
 *            +193376413692421*a(n-9) -161723079970872*a(n-10)
 *            +46496477673360*a(n-11), with the i...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    static seq_t init[12];
    static bool ready = false;
    if (!ready)
    {
        /* 0th entry unused but initialized to zero for safety */
        init[0] = (seq_t)0;
        init[1] = (seq_t)4;
        init[2] = (seq_t)354;
        init[3] = (seq_t)175884;
        init[4] = (seq_t)188834753;
        init[5] = seq_from_str("253155125338");
        init[6] = seq_from_str("356734116034266");
        init[7] = seq_from_str("508116551730888266");
        init[8] = seq_from_str("725424852023224352701");
        init[9] = seq_from_str("1036196988923824970841964");
        init[10] = seq_from_str("1480269108515044158376950586");
        init[11] = seq_from_str("2114704441343238968833690030736");
        ready = true;
    }

    if (n <= 11)
        return init[n];

    /* Shift registers for the last 11 computed values */
    seq_t a1 = init[1], a2 = init[2], a3 = init[3], a4 = init[4];
    seq_t a5 = init[5], a6 = init[6], a7 = init[7], a8 = init[8];
    seq_t a9 = init[9], a10 = init[10], a11 = init[11];

    for (idx_t i = 12; i <= n; ++i)
    {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;

        pos += (seq_t)1953 * a11;
        pos += (seq_t)119749880 * a9;
        pos += (seq_t)211005732306 * a7;
        pos += (seq_t)24791539817472 * a5;
        pos += (seq_t)193376413692421 * a3;
        pos += (seq_t)46496477673360 * a1;

        neg += (seq_t)829512 * a10;
        neg += (seq_t)7217451834 * a8;
        neg += (seq_t)3163113776744 * a6;
        neg += (seq_t)99982144638429 * a4;
        neg += (seq_t)161723079970872 * a2;

        seq_t next = pos - neg;

        a1 = a2;  a2 = a3;  a3 = a4;  a4 = a5;  a5 = a6;
        a6 = a7;  a7 = a8;  a8 = a9;  a9 = a10; a10 = a11; a11 = next;
    }

    return a11;
}
