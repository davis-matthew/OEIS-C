/*
 * A045062  Numbers whose base-4 representation contains no 0's and exactly
 *          one 3.
 * https://oeis.org/A045062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer whose base-4 digits consist only of *
 *            1,2,3 and contain exactly one digit 3. The sequence is generated
 *            by * combinatorial unranking of the lexicographically ordered
 *            set of such * digit strings
 * Range:     u8 n <= 49, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow2(uint64_t k) { return (seq_t)1 << k; }

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n < 1, but safe to return 0 */

    /* Find the length L of the base‑4 representation of a(n). */
    unsigned long long N0 = (unsigned long long)(n - 1);     /* zero‑based overall index */
    unsigned __int128 cumulative = 0;
    unsigned L = 0;
    for (unsigned k = 1;; ++k) {
        unsigned __int128 blockCount =
            (unsigned __int128)k * ((unsigned __int128)1 << (k - 1));
        if (cumulative + blockCount > (unsigned __int128)N0) {
            L = k;
            break;
        }
        cumulative += blockCount;
    }

    unsigned long long t = N0 - (unsigned long long)cumulative;   /* index within length L */

    /* Unrank the t‑th lexicographic string of length L over {1,2,3}
     * with exactly one occurrence of 3. */
    unsigned int digits[64];          /* L will never exceed 64 for n up to 10000 */
    int has3 = 0;                      /* 0 = 3 not yet used, 1 = used */

    for (unsigned i = 0; i < L; ++i) {
        unsigned R = L - i;            /* remaining positions including current */
        unsigned chosen = 0;
        for (unsigned d = 1; d <= 3; ++d) {
            if (d == 3 && has3) continue;   /* cannot place second 3 */

            unsigned long long cnt;
            if (has3) {          /* 3 already placed, remaining digits 1 or 2 */
                cnt = 1ULL << (R - 1);
            } else {              /* 3 not yet placed */
                if (d == 3) {
                    cnt = 1ULL << (R - 1);
                } else {          /* d is 1 or 2, place 3 later */
                    if (R - 1 == 0)
                        cnt = 0;
                    else
                        cnt = (R - 1) * (1ULL << (R - 2));
                }
            }

            if (t < cnt) {       /* this digit is the correct one */
                digits[i] = (unsigned) d;
                if (d == 3) has3 = 1;
                chosen = 1;
                break;
            } else {
                t -= cnt;        /* skip this block */
            }
        }
        /* For safety; the loop must always find a digit. */
        if (!chosen) return 0;
    }

    /* Convert the digit array to its decimal value. */
    seq_t val = 0;
    for (unsigned i = 0; i < L; ++i)
        val = val * 4 + digits[i];

    return val;
}
