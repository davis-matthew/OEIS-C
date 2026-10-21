/*
 * A230964  Number of n X 2 0..3 arrays x(i,j) with each element horizontally,
 *          vertically, diagonally or antidiagonally next to at least one
 *          element with value (x(i,j)+1) mod 4 and at least one element with
 *          value (x(i,j)-1) mod 4, and upper left element zero.
 * https://oeis.org/A230964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            7*a(n-1)+10*a(n-2)+38*a(n-3)-132*a(n-4)-232*a(n-5)-448*a(n-6)
 *            +320*a(n-7)+256*a(n-8) Initial values (n = 1 … 8): a(1)=0 ,
 *            a(2)=6 , a(3)=22 , a(4)=200 , a(5)=1732 , a(6)=14384 ,
 *            a(7)=121696 , a(8)=1027464
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* 8 initial values computed once */
    static seq_t init[8];
    static bool init_done = false;

    if (!init_done) {
        init[0] = seq_from_str("0");
        init[1] = seq_from_str("6");
        init[2] = seq_from_str("22");
        init[3] = seq_from_str("200");
        init[4] = seq_from_str("1732");
        init[5] = seq_from_str("14384");
        init[6] = seq_from_str("121696");
        init[7] = seq_from_str("1027464");
        init_done = true;
    }

    if (n <= 8)
        return init[n-1];

    /* Sliding window of the last eight terms */
    seq_t arr[8];
    for (idx_t i = 0; i < 8; ++i)
        arr[i] = init[i];

    for (idx_t i = 9; i <= n; ++i) {
        seq_t pos = arr[7]*7 + arr[6]*10 + arr[5]*38 + arr[1]*320 + arr[0]*256;
        seq_t neg = arr[4]*132 + arr[3]*232 + arr[2]*448;
        seq_t next = pos - neg;  /* guaranteed non‑negative for n ≥ 9 */

        /* shift left and insert new term */
        for (idx_t k = 0; k < 7; ++k)
            arr[k] = arr[k+1];
        arr[7] = next;
    }

    return arr[7];
}
