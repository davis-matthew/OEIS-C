/*
 * A144179  Table in which row n lists the digits of n, the digits of the
 *          number of days in year n and if n is a leap year the digits of the
 *          number of days in February of that year.
 * https://oeis.org/A144179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, base, less
 * Formula:   a(n) is the n-th digit in the concatenation of [digits of year],
 *            [digits of days in year (365 or 366)], and if the year is leap,
 *            [digits of days in February (=29)]. Example: row for 4 ->
 *            4,3,6,6,2,9 gives digits 4,3,6,6,2,9
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Pre‑computed powers of ten up to 10^19 (fits in uint64_t). */
static const uint64_t pow10[20] = {
    1ULL,          10ULL,          100ULL,
    1000ULL,       10000ULL,       100000ULL,
    1000000ULL,    10000000ULL,    100000000ULL,
    1000000000ULL, 10000000000ULL, 100000000000ULL,
    1000000000000ULL, 10000000000000ULL, 100000000000000ULL,
    1000000000000000ULL, 10000000000000000ULL, 100000000000000000ULL,
    1000000000000000000ULL, 10000000000000000000ULL
};

/* Compute cumulative number of digits up to year m. */
static idx_t cum_digits(idx_t m)
{
    /* sum of digits of all numbers from 1..m */
    unsigned __int128 sumDigits = 0;
    int d = 1;
    for (uint64_t start = 1; start <= m; ++d) {
        uint64_t end = (d < 20 && pow10[d] - 1 < m) ? pow10[d] - 1 : m;
        if (end >= start) {
            sumDigits += (unsigned __int128)(end - start + 1) * d;
        }
        if (d == 20) break;     /* prevent overflow on pow10[d] */
        start = pow10[d];
    }

    unsigned __int128 singleYear = (unsigned __int128)3 * m;
    unsigned __int128 leapCnt = (unsigned __int128)(m / 4 - m / 100 + m / 400) * 2;
    unsigned __int128 total = sumDigits + singleYear + leapCnt;
    return (idx_t)(total);
}

/* Return n-th digit of the sequence, n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defined for safety */

    /* Find the year Y that contains the n-th digit via binary search. */
    idx_t low = 1;
    idx_t high = n;                /* at least one digit per year */
    while (cum_digits(high) < n) {
        if (high > ~((idx_t)1) / 2) { high = ~((idx_t)0); break; }
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cum_digits(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t Y = low;

    idx_t prev = (Y > 1 ? cum_digits(Y - 1) : 0);
    idx_t offset = n - prev - 1;   /* 0‑based offset within the row */

    /* digits of year Y */
    uint8_t digitsY[20];
    int lenY = 0;
    uint64_t tmp = Y;
    uint8_t rev[20];
    int revLen = 0;
    if (tmp == 0) {
        rev[revLen++] = 0;
    } else {
        while (tmp > 0) {
            rev[revLen++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }
    lenY = revLen;
    for (int i = 0; i < revLen; ++i) {
        digitsY[i] = rev[revLen - 1 - i];
    }

    bool isleap = ((Y % 4 == 0 && Y % 100 != 0) || (Y % 400 == 0));

    if (offset < lenY) {
        return (seq_t)digitsY[offset];
    }
    offset -= lenY;

    /* days in year (365 or 366) */
    static const uint8_t days_nonleap[3] = {3, 6, 5};
    static const uint8_t days_leap[3]  = {3, 6, 6};
    if (offset < 3) {
        return (seq_t)(isleap ? days_leap[offset] : days_nonleap[offset]);
    }
    offset -= 3;

    /* days in February for leap years (29) */
    if (isleap) {
        static const uint8_t feb_digits[2] = {2, 9};
        return (seq_t)feb_digits[offset];
    }

    /* Should never reach here */
    return (seq_t)0;
}
