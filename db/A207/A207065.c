/*
 * A207065  Number of n X 5 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) for n>6, * with a(1)=14, a(2)=196, a(3)=896, a(4)=2688,
 *            a(5)=6398, a(6)=13132
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 107, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Cache of terms up to some maximum (≈10000) for quick repeated access. */
static seq_t cache[10001];
static idx_t computed_until = 0;

static void init_cache(void)
{
    cache[1] = (seq_t)14;
    cache[2] = (seq_t)196;
    cache[3] = (seq_t)896;
    cache[4] = (seq_t)2688;
    cache[5] = (seq_t)6398;
    cache[6] = (seq_t)13132;
    computed_until = 6;
}

static void compute_up_to(idx_t n)
{
    if (computed_until == 0) init_cache();
    if (n <= computed_until) return;
    if (n > (idx_t)10000) n = (idx_t)10000;   /* safe bound per limits */

    for (idx_t i = computed_until + 1; i <= n; ++i) {
        cache[i] = cache[i-1] * 6
                  - cache[i-2] * 15
                  + cache[i-3] * 20
                  - cache[i-4] * 15
                  + cache[i-5] * 6
                  - cache[i-6];
    }
    computed_until = n;
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
