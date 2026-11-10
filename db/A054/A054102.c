/*
 * A054102  T(n,n-2), array T as in A054098.
 * https://oeis.org/A054102
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n,n-2), where T(n,0)=1, T(1,1)=2, T(n,1)=r(n-1), T(n,k)
 *            = T(n,k-1) + (-1)^(k+1)*r(n-k) for 2<=k<=n, r(h) = sum of row h;
 *            r(0)=1, r(1)=3
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 19 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A054102_rs = NULL;   /* row sums */
static seq_t *A054102_av = NULL;   /* T(r, r-2) for each row r */
static size_t A054102_cnt = 0;
static size_t A054102_cap = 0;

static void A054102_ensure(size_t n)
{
    if (n + 1 > A054102_cap) {
        size_t nc = A054102_cap ? A054102_cap : 64;
        while (nc < n + 1) nc *= 2;
        A054102_rs = mem_resize(seq_t, A054102_rs, nc);
        A054102_av = mem_resize(seq_t, A054102_av, nc);
        A054102_cap = nc;
    }
    while (A054102_cnt <= n) {
        size_t r = A054102_cnt;
        if (r == 0) {
            A054102_rs[0] = 1;
            A054102_av[0] = 0;
        } else if (r == 1) {
            A054102_rs[1] = 3;
            A054102_av[1] = 0;
        } else {
            seq_t cur = A054102_rs[r - 1];      /* T(r,1) */
            seq_t sum = 1 + cur;                /* T(r,0) + T(r,1) */
            seq_t target = 0;
            if (r == 2) target = 1;             /* T(2,0) */
            if (r == 3) target = cur;           /* T(3,1) */
            for (size_t k = 2; k <= r; k++) {
                /* (-1)^(k+1): -1 for even k, +1 for odd k */
                if (k & 1) cur = cur + A054102_rs[r - k];
                else       cur = cur - A054102_rs[r - k];
                sum += cur;
                if (k + 2 == r) target = cur;
            }
            A054102_rs[r] = sum;
            A054102_av[r] = target;
        }
        A054102_cnt++;
    }
}

seq_t a(idx_t n)
{
    A054102_ensure((size_t)n);
    return A054102_av[(size_t)n];
}
