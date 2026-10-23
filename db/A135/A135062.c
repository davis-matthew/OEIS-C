/*
 * A135062  Define the sequence {b_n(m)} by b_n(0)=1; b_n(m) =
 *          A000005(b_n(m-1) + n), for all m >= 1. Then a(n) is the smallest
 *          positive integer such that b_n(m) = b_n(m + a(n)) for all m > some
 *          positive integer.
 * https://oeis.org/A135062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of eventual cycle of the sequence b_n defined by
 *            b_n(0)=1 and b_n(m)=d(b_n(m‑1)+n), where d(x) is the number of
 *            divisors of x. a(n) is the minimal positive integer L such that
 *            for all sufficiently large m, b_n(m)=b_n(m+L)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute the number of divisors of a non‑negative seq_t value n.
   Uses trial division up to sqrt(n).  Works for the typical integer
   sizes used in the project (uint8_t … uint64_t) and also for
   arbitrary‑precision seq_t where operators + - * / % and comparison
   are defined. */
static seq_t divisor_count(seq_t n)
{
    if (n <= (seq_t)1) return (seq_t)1;

    seq_t k = n;
    seq_t result = (seq_t)1;

    for (idx_t i = 2; ; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t i_sq = i_seq * i_seq;
        if (i_sq > k) break;

        int exp = 0;
        while (k % i_seq == 0) {
            k = k / i_seq;
            ++exp;
        }
        if (exp > 0)
            result = result * (seq_t)(exp + 1);
    }

    if (k > (seq_t)1)
        result = result * (seq_t)2;

    return result;
}

/* One step of the recurrence: b_n(m) = d(b_n(m‑1)+n). */
static seq_t step_f(seq_t x, idx_t n)
{
    seq_t y = x + (seq_t)n;
    return divisor_count(y);
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* Special case n = 0 is handled by the general algorithm,
       but returning directly can avoid a tiny loop. */
    if (n == (idx_t)0) return (seq_t)1;

    /* Floyd's cycle detection. */
    seq_t tortoise = step_f((seq_t)1, n);
    seq_t hare = step_f(tortoise, n);

    while (tortoise != hare) {
        tortoise = step_f(tortoise, n);
        hare = step_f(step_f(hare, n), n);
    }

    /* Find the start of the cycle (mu).  Not needed for λ, but
       required to position tortoise at the cycle entry. */
    seq_t tort = (seq_t)1;
    while (tort != hare) {
        tort = step_f(tort, n);
        hare = step_f(hare, n);
    }

    /* Find the cycle length λ. */
    seq_t hare2 = step_f(tort, n);
    seq_t lambda = (seq_t)1;
    while (tort != hare2) {
        hare2 = step_f(hare2, n);
        lambda = lambda + (seq_t)1;
    }

    return lambda;
}
