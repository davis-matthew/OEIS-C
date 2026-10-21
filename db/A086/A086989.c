/*
 * A086989  Sum of even-indexed divisors of n.
 * https://oeis.org/A086989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the divisors of n that lie at even positions
 *            (1‑based) in the sorted list of positive divisors of n
 * Range:     u8 n <= 167, u16 n <= 9777, u32 n <= 9777, u64 n <= 9777,
 *            u128 n <= 9777, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Example: n=10 → divisors 1,2,5,10; even positions 2 and 10 → a(10)=12. */

#include <stdlib.h>
#include <stdint.h>

/* Comparator that orders uint64_t values in ascending order. */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* Compute the sum of even‑indexed divisors of n. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* a(1)=0 is handled automatically */

    /* Max number of divisors for n ≤ 10000 is < 200; array big enough. */
    uint64_t divs[256];
    int cnt = 0;

    idx_t i;
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[cnt++] = (uint64_t)i;
            if (i != n / i)          /* avoid duplicate when n is a perfect square */
                divs[cnt++] = (uint64_t)(n / i);
        }
    }

    /* Sort the list of divisors in ascending order. */
    qsort(divs, cnt, sizeof(uint64_t), cmp_uint64);

    seq_t sum = 0;
    for (int j = 1; j < cnt; j += 2)   /* even positions (1‑based) are at 0‑based indices 1,3,5,… */
        sum += (seq_t)divs[j];

    return sum;
}
