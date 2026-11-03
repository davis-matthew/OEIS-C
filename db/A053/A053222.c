/*
 * A053222  First differences of sigma(n).
 * https://oeis.org/A053222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = σ(n+1) – σ(n) (first differences of the sum‑of‑divisors
 *            function)
 * Range:     i8 n <= 82, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9556 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static __int128 sum_of_divisors_uint64(uint64_t m)
{
    __int128 sum = 0;
    uint64_t i = 1;
    /* Loop until i*i > m; m ≤ 10⁴ in the test harness, so this is fast. */
    while (i * i <= m) {
        if (m % i == 0) {
            sum += i;
            uint64_t j = m / i;
            if (j != i)          /* avoid double‑counting a square root */
                sum += j;
        }
        ++i;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n = 0 the values are
       irrelevant but we return 0 to keep the function well‑defined. */
    if (n == 0)
        return 0;

    uint64_t n1 = (uint64_t)n;
    uint64_t n2 = n1 + 1U;
    __int128 s1 = sum_of_divisors_uint64(n1);
    __int128 s2 = sum_of_divisors_uint64(n2);
    return (seq_t)(s2 - s1);
}
