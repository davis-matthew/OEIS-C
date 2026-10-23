/*
 * A179620  a(n) = largest k such that A002808(n+1) = A002808(n) + (A002808(n)
 *          mod k), or 0 if no such k exists.
 * https://oeis.org/A179620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest k such that A002808(n+1) = A002808(n) +
 *            (A002808(n) mod k) Computed via composite gaps: let C_n = n-th
 *            composite, diff = C_{n+1} - C_n. Then a(n) = C_n - diff if diff
 *            < C_n - diff, otherwise 0
 * Range:     u8 n <= 200, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

static seq_t find_nth_composite(idx_t n)
{
    seq_t cur = 4;          /* first composite number */
    idx_t count = 1;        /* composite index */
    while (count < n) {
        ++cur;
        while (is_prime((idx_t)cur))
            ++cur;
        ++count;
    }
    return cur;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n < 1, return 0 */

    /* Find the n-th and (n+1)-th composite numbers */
    seq_t Cn   = find_nth_composite(n);
    seq_t next = Cn + 1;
    while (is_prime((idx_t)next))
        ++next;                  /* C_{n+1} */

    seq_t diff = next - Cn;
    seq_t k    = Cn - diff;

    return (diff < k) ? k : 0;
}
