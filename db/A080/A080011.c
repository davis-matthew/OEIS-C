/*
 * A080011  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=2, r=3, I={-1}.
 * https://oeis.org/A080011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-1)+a(n-2)+a(n-3)+2*a(n-4)+3*a(n-5)-a(n-6)+a(n-8)-a(n-9)-a(n-10),
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=3, a(4)=7, a(5)=15, a(6)=29,
 *            a(7)=59, a(8)=126, a(9)=262
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *mem = NULL;
    static idx_t sz = 0;
    if (n >= sz) {
        idx_t new_sz = n + 1;
        /* realloc may return void*; cast explicitly for C++ */
        seq_t *tmp = (seq_t *)realloc(mem, new_sz * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failed */
        mem = tmp;
        for (idx_t i = sz; i < new_sz; ++i) {
            if (i < 10) {
                static const seq_t init[10] = {1, 1, 1, 3, 7, 15, 29, 59, 126, 262};
                mem[i] = init[i];
            } else {
                mem[i] = mem[i-1] + mem[i-2] + mem[i-3]
                            + 2 * mem[i-4] + 3 * mem[i-5]
                            - mem[i-6] + mem[i-8]
                            - mem[i-9] - mem[i-10];
            }
        }
        sz = new_sz;
    }
    return mem[n];
}
