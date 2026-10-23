/*
 * A142600  Third trisection of A061037.
 * https://oeis.org/A142600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n % 4 == 1: 36*((n-1)/4)^2 + 24*((n-1)/4) + 3 n % 4 == 2:
 *            144*((n-2)/4)^2 + 168*((n-2)/4) + 45 n % 4 == 3: 9*((n-3)/4)^2 +
 *            15*((n-3)/4) + 6 n % 4 == 0: 144*(n/4)^2 + 24*(n/4) - 3
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long r = n & 3ULL;
    unsigned long long k, res;

    switch (r) {
        case 1:
            k = (n - 1) / 4;
            res = 36ULL * k * k + 24ULL * k + 3ULL;
            break;
        case 2:
            k = (n - 2) / 4;
            res = 144ULL * k * k + 168ULL * k + 45ULL;
            break;
        case 3:
            k = (n - 3) / 4;
            res = 9ULL * k * k + 15ULL * k + 6ULL;
            break;
        case 0:
            k = n / 4;
            res = 144ULL * k * k + 24ULL * k - 3ULL;
            break;
    }

    return (seq_t)res;
}
