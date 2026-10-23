/*
 * A045152  Numbers whose base-4 representation contains exactly three 2's and
 *          four 3's.
 * https://oeis.org/A045152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose base‑4 representation contains exactly three 2's
 *            and four 3's. The function enumerates such numbers in increasing
 *            order
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute binomial coefficient C(n,k) for small n (n ≤ 10 here). */
static uint64_t choose(uint64_t n, uint64_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    uint64_t res = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

/* Number of strings of length len that still need rem2 twos and rem3 threes
   (the remaining positions may be 0 or 1). */
static uint64_t count_suffix(int len, int rem2, int rem3)
{
    if (rem2 < 0 || rem3 < 0) return 0;
    int need = rem2 + rem3;
    if (len < need) return 0;
    uint64_t ways_pos    = choose(len, need);
    uint64_t ways_assign = choose(need, rem2);
    int free = len - need;
    uint64_t ways_free = 1ULL << free;          /* 2^free, free ≤ 3 */
    return ways_pos * ways_assign * ways_free;
}

/* Number of valid numbers with total length L. */
static uint64_t count_len(int L)
{
    if (L < 7) return 0;
    uint64_t ways_pos    = choose(L, 7);
    uint64_t ways_assign = 35ULL;               /* C(7,3) */
    uint64_t ways_free   = 1ULL << (L - 7);     /* 2^(L-7), L ≤ 10 */
    return ways_pos * ways_assign * ways_free;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1 */

    /* Find the length L such that the first n terms lie within lengths ≤ L. */
    uint64_t cum = 0;
    int L = 7;
    while (true) {
        uint64_t cnt = count_len(L);
        if (cum + cnt >= n)
            break;
        cum += cnt;
        ++L;
    }

    uint64_t k = n - cum - 1;             /* zero‑based index inside this length */
    uint8_t digits[10];                   /* maximum length needed for L ≤ 10 */
    int rem2 = 3, rem3 = 4;               /* twos and threes yet to place */

    for (int pos = 0; pos < L; ++pos) {
        int start = (pos == 0) ? 1 : 0;    /* no leading zero in base‑4 */
        for (int d = start; d <= 3; ++d) {
            int r2 = rem2, r3 = rem3;
            if (d == 2) --r2;
            else if (d == 3) --r3;
            int rem_len = L - pos - 1;
            uint64_t cnt = count_suffix(rem_len, r2, r3);
            if (k < cnt) {
                digits[pos] = (uint8_t)d;
                rem2 = r2;
                rem3 = r3;
                break;
            } else {
                k -= cnt;
            }
        }
    }

    /* Convert the base‑4 digits to the decimal value. */
    seq_t val = 0;
    for (int i = 0; i < L; ++i) {
        val = (val << 2) | (seq_t)digits[i];
    }
    return val;
}
