/*
 * A078339  Let u(1)=u(2)=u(3)=1 and u(n)=(-1)^n*sign(u(n-1)-u(n-2))*u(n-3);
 *          then a(n)=sum(k=1,n,sum(i=1,k,u(i))) - 3*(n-1).
 * https://oeis.org/A078339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum(k=1..n,sum(i=1..k,u(i))) - 3*(n-1), where
 *            u(1)=u(2)=u(3)=1 and u(n)=(-1)^n * sign(u(n-1)-u(n-2)) * u(n-3)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>


static seq_t a_table[18];
static bool a_table_init = false;

static void init_a_table(void)
{
    if (a_table_init)
        return;

    /* compute u(1)..u(18) in signed 64‑bit integers */
    int64_t u[19];
    u[1] = 1; u[2] = 1; u[3] = 1;
    for (idx_t n = 4; n <= 18; ++n) {
        int parity = (n & 1) ? -1 : 1;       /* (-1)^n */
        int64_t diff = u[n - 1] - u[n - 2];
        int sign = 0;
        if (diff > 0)      sign =  1;
        else if (diff < 0) sign = -1;
        u[n] = (int64_t)parity * sign * u[n - 3];
    }

    /* cumulative sums b(k) = Σ_{i=1}^k u(i) */
    int64_t b[19] = {0};
    for (idx_t k = 1; k <= 18; ++k)
        b[k] = b[k - 1] + u[k];

    /* cumulative sums of b(k) and final a(k) = Σ b(k) - 3*(k-1) */
    int64_t s[19] = {0};
    for (idx_t k = 1; k <= 18; ++k) {
        s[k] = s[k - 1] + b[k];
        a_table[k - 1] = (seq_t)(s[k] - 3 * (int64_t)(k - 1));
    }

    a_table_init = true;
}

seq_t a(idx_t n)
{
    init_a_table();
    if (n == 0)
        return 0;          /* defined as 0 for safety, offset starts at 1 */
    idx_t idx = (n - 1) % 18;   /* period 18 */
    return a_table[idx];
}
