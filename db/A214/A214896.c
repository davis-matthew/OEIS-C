/*
 * A214896  Conway's subprime Fibonacci sequence starting with (382, 127).
 * https://oeis.org/A214896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=382, a(2)=127; for n>2 a(n) =
 *            (a(n-1)+a(n-2))/lpf(a(n-1)+a(n-2)), where lpf(x) is the least
 *            prime factor of x (if x is prime, lpf(x)=x, so
 *            a(n)=a(n-1)+a(n-2))
 * Range:     u8 none, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t least_prime_factor(uint64_t n)
{
    if (n <= 1) return n;                /* 1 or 0, no meaningful factor */
    for (uint64_t d = 2; d * d <= n; ++d)
        if (n % d == 0)
            return d;                    /* first divisor found -> least prime factor */
    return n;                            /* n is prime */
}

static seq_t init[10];
static bool built = false;

static void build_initial(void)
{
    seq_t a0 = (seq_t)382;
    seq_t a1 = (seq_t)127;
    init[0] = a0;
    init[1] = a1;

    for (int i = 2; i < 10; ++i)
    {
        seq_t sum = init[i - 1] + init[i - 2];
        uint64_t sum64 = (uint64_t)sum;     /* all sequence values fit in 64‑bit */
        uint64_t lpf = least_prime_factor(sum64);
        if (lpf != sum64)                  /* composite number */
            init[i] = sum / (seq_t)lpf;
        else                               /* prime number */
            init[i] = sum;
    }
    built = true;
}

seq_t a(idx_t n)
{
    if (!built)
        build_initial();

    if (n == 0)            /* undefined for n<1, but return 0 as sentinel */
        return (seq_t)0;
    return init[(n - 1) % 10];
}
