/*
 * A231282  Number of nX5 0..3 arrays x(i,j) with each element horizontally or
 *          antidiagonally next to at least one element with value (x(i,j)+1)
 *          mod 4, and upper left element zero.
 * https://oeis.org/A231282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n-1)+422*a(n-2)+112*a(n-3)-13713*a(n-4)-12558*a(n-5)
 *            +109956*a(n-6)-205672*a(n-7)+110336*a(n-8)+3309440*a(n-9)-849920*a(n-10)
 *            for n>11, with a(1)=0, a(2)=26, a(3)=834, a(4)=34786,
 *            a(5)=1400418, a(6)=56442770, a(7)=2276141330, a(8)=91775666754,
 *            a(9)=3700571551586, a(10)=149213472628178...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *mem = NULL;
    static idx_t memsize = 0;

    if (n == 0) return 0;          /* offset starts at 1 */

    if (n <= memsize) return mem[n-1];

    seq_t *newmem = (seq_t *)realloc(mem, n * sizeof(seq_t));
    if (newmem == NULL) return 0;   /* allocation failure – unlikely in tests */
    mem = newmem;

    for (idx_t i = memsize + 1; i <= n; ++i) {
        if (i == 1)          mem[i-1] = 0;
        else if (i == 2)     mem[i-1] = 26;
        else if (i == 3)     mem[i-1] = 834;
        else if (i == 4)     mem[i-1] = 34786;
        else if (i == 5)     mem[i-1] = 1400418;
        else if (i == 6)     mem[i-1] = 56442770;
        else if (i == 7)     mem[i-1] = 2276141330;
        else if (i == 8)     mem[i-1] = 91775666754;
        else if (i == 9)     mem[i-1] = 3700571551586;
        else if (i == 10)    mem[i-1] = 149213472628178;
        else if (i == 11)    mem[i-1] = 6016549721672594;
        else {
            seq_t pos = 0, neg = 0;
            pos += ((seq_t)30) * mem[i-2];
            pos += ((seq_t)422) * mem[i-3];
            pos += ((seq_t)112) * mem[i-4];
            pos += ((seq_t)109956) * mem[i-7];
            pos += ((seq_t)110336) * mem[i-9];
            pos += ((seq_t)3309440) * mem[i-10];
            neg += ((seq_t)13713) * mem[i-5];
            neg += ((seq_t)12558) * mem[i-6];
            neg += ((seq_t)205672) * mem[i-8];
            neg += ((seq_t)849920) * mem[i-11];
            mem[i-1] = pos - neg;
        }
    }

    memsize = n;
    return mem[n-1];
}
