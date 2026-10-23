/*
 * A045154  Numbers whose base-4 representation contains exactly four 2's and
 *          one 3.
 * https://oeis.org/A045154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth number whose base‑4 representation contains exactly
 *            four 2's and one 3
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* binomial coefficient for small n   */
static inline uint64_t choose_uint64(uint64_t n, uint64_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    uint64_t res = 1;
    for (uint64_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

/* number of admissible base‑4 numbers of length L   */
static inline uint64_t count_length(uint64_t L)
{
    if (L < 5) return 0;
    uint64_t c = choose_uint64(L - 1, 4);   /* C(L-1,4)  */
    uint64_t prod = L * c;                  /* L*C(L-1,4) */
    prod <<= (L - 5);                       /* *2^(L-5)  */
    return prod;
}

seq_t a(idx_t n)
{
    uint64_t remaining = n;        /* remaining rank to locate               */
    uint64_t len = 5;              /* current length of the base‑4 string     */

    /* find the length where the nᵗʰ term lies */
    while (true) {
        uint64_t cnt = count_length(len);
        if (remaining <= cnt) break;
        remaining -= cnt;
        ++len;
    }

    uint64_t idx = remaining - 1;   /* 0‑based index within this length       */
    uint64_t r3 = 1, r2 = 4;        /* remaining 3's and 2's needed            */
    uint64_t val = 0;                /* build answer                               */

    for (uint64_t pos = 0; pos < len; ++pos) {
        for (uint32_t d = 0; d < 4; ++d) {
            if (pos == 0 && d == 0) continue;      /* no leading zero */
            if (d == 3 && r3 == 0) continue;       /* no 3 left    */
            if (d == 2 && r2 == 0) continue;       /* no 2 left    */

            uint64_t r3p = r3 - (d == 3);
            uint64_t r2p = r2 - (d == 2);
            uint64_t m   = len - pos - 1;           /* remaining positions              */
            if (m < r3p + r2p) continue;            /* impossible to place specials    */

            uint64_t c1   = choose_uint64(m, r3p);
            uint64_t c2   = choose_uint64(m - r3p, r2p);
            uint64_t pow2 = 1ULL << (m - r3p - r2p);
            uint64_t completions = c1 * c2 * pow2;

            if (completions <= idx) {
                idx -= completions;               /* skip this block */
                continue;
            }

            /* choose digit d */
            val = val * 4 + d;
            r3 = r3p;
            r2 = r2p;
            break;
        }
    }

    return (seq_t)val;
}
