/*
 * A066924  Nonprime numbers n such that the GCD of n and the Chowla's
 *          function of n (A048050) is >= n/3.
 * https://oeis.org/A066924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search: a(n) = n-th nonprime k >= 1 with 3*gcd(k, A048050(k)) >=
 *            k, where A048050(k) = sigma(k)-k-1 (0 for k=1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A066924_chowla(uint64_t n)
{
    if (n <= 2)
        return 0;
    uint64_t sum = 0;
    for (uint64_t i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            sum += i;
            uint64_t j = n / i;
            if (j != i)
                sum += j;
        }
    }
    return sum;
}

static uint64_t A066924_gcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    static uint64_t terms[256];
    static unsigned cnt = 0;
    static uint64_t next_k = 1;

    if (n > 255)
        n = 255;
    while (cnt < (unsigned)n) {
        uint64_t k = next_k++;
        uint64_t c = A066924_chowla(k);
        if (k >= 2 && c == 0)
            continue; /* prime */
        uint64_t g = A066924_gcd(k, c);
        if (3 * g >= k)
            terms[cnt++] = k;
    }
    return (seq_t)terms[n - 1];
}
