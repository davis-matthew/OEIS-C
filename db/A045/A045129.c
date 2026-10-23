/*
 * A045129  Numbers whose base-4 representation contains exactly four 1's and
 *          no 3's.
 * https://oeis.org/A045129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer whose base‑4 representation contains exactly
 *            four digits equal to 1 and no digit equal to 3
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* binomial coefficient C(n,k) (n,k <= 64).  Uses unsigned long long. */
static unsigned long long binom(unsigned int n, unsigned int k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    unsigned long long res = 1;
    for (unsigned int i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

/* Count of admissible length‑len base‑4 numbers: exactly four 1’s,
   no 3’s, leading digit non‑zero. */
static unsigned long long count_len(unsigned int len) {
    if (len < 4) return 0;
    unsigned long long total = binom(len, 4) * (1ULL << (len - 4));
    unsigned long long sub = 0;
    if (len >= 5) {
        sub = binom(len - 1, 4) * (1ULL << (len - 5)); /* leading zero */
    }
    return total - sub;
}

/* Main sequence function: a(idx_t n) */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset starts at 1 */

    /* Find the length L of the number that contains the n‑th element. */
    unsigned long long cumulative = 0;
    unsigned int len = 0;
    for (;;) {
        ++len;
        if (len < 4) continue;
        unsigned long long cnt = count_len(len);
        if (cumulative + cnt >= n) break;
        cumulative += cnt;
    }

    /* k is the position within all length‑len numbers. */
    unsigned long long k = n - cumulative;

    /* Construct the base‑4 digits lexicographically. */
    idx_t rem = len;        /* remaining positions to fill */
    int r = 4;              /* remaining ones to place */
    seq_t result = 0;

    for (idx_t pos = 0; pos < len; ++pos) {
        for (int d = 0; d <= 2; ++d) {
            if (pos == 0 && d == 0) continue;   /* no leading zero */

            int r_needed = r - (d == 1);
            idx_t rem_after = rem - 1;

            /* Count numbers possible if we choose digit d here. */
            unsigned long long cnt_possible = 0;
            if (r_needed >= 0) {
                if (rem_after >= (idx_t)r_needed) {
                    if (r_needed == 0) {
                        cnt_possible = 1ULL << rem_after; /* all 0/2 */
                    } else {
                        cnt_possible = binom((unsigned int)rem_after,
                                             (unsigned int)r_needed)
                                       * (1ULL << (rem_after - r_needed));
                    }
                }
            }

            if (k <= cnt_possible) {
                result = result * 4 + (seq_t)d;
                r = r_needed;
                rem = rem_after;
                break;
            } else {
                k -= cnt_possible;
            }
        }
    }

    return result;
}
