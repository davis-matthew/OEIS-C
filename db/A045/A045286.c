/*
 * A045286  Numbers whose base-5 representation contains exactly two 2's and
 *          one 4.
 * https://oeis.org/A045286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A045286: numbers whose base‑5 representation contains exactly
   two 2's and one 4.  The function enumerates numbers by length,
   determines the length containing the nth term, and then builds the
   number digit by digit using combinatorial unranking.  The implementation
   works for any seq_t big‑integer type and uses only the allowed operators. */
#include <stdint.h>

/* 3^e for unsigned e.  Used for the choices of the remaining digits. */
static seq_t pow3_uint64(unsigned e)
{
    seq_t r = 1;
    for (unsigned i = 0; i < e; ++i)
        r *= 3;
    return r;
}

/* binomial coefficient C(n,k) for n ≤ 64.  Result is computed using only
   integer arithmetic, so it is exact for all supported seq_t types. */
static seq_t binom_uint64(unsigned n, unsigned k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (unsigned i = 1; i <= k; ++i)
        r = r * (seq_t)(n + 1 - i) / i;
    return r;
}

/* Number of valid base‑5 numbers of length L (L ≥ 3) with leading digit > 0. */
static seq_t count_len(unsigned L)
{
    if (L < 3) return 0;
    if (L == 3) return 3;                /* 224, 242, 422 */

    seq_t total = binom_uint64(L, 2) *
                  (seq_t)(L - 2) *
                  pow3_uint64(L - 3);

    /* subtract sequences whose leading digit is 0 */
    seq_t bad = binom_uint64(L - 1, 2) *
                (seq_t)(L - 3) *
                pow3_uint64(L - 4);
    return total - bad;
}

/* Number of completions for the remaining positions when we must place
   exactly twos_left 2's and fours_left 4's.  The other positions may be
   0,1 or 3 (3 choices each). */
static seq_t count_completions(unsigned rem,
                               unsigned twos_left,
                               unsigned fours_left)
{
    if (twos_left > rem || fours_left > rem ||
        twos_left + fours_left > rem) return 0;

    seq_t part = binom_uint64(rem, twos_left) *
                 binom_uint64(rem - twos_left, fours_left);
    part *= pow3_uint64(rem - twos_left - fours_left);
    return part;
}

/* External linkage required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* 1. Locate the length L that contains the nth term. */
    seq_t acc = 0;
    unsigned L = 3;
    while (1) {
        seq_t cnt = count_len(L);
        if (acc + cnt >= (seq_t)n)
            break;
        acc += cnt;
        ++L;
    }

    /* 2. Zero‑based index within numbers of length L. */
    seq_t k = (seq_t)n - acc - 1;

    /* 3. Construct the number digit by digit. */
    unsigned twos  = 2;
    unsigned fours = 1;
    seq_t result = 0;

    for (unsigned pos = 0; pos < L; ++pos) {
        unsigned rem = L - pos - 1;
        for (int d = (pos == 0 ? 1 : 0); d <= 4; ++d) {
            if (d == 2 && twos == 0) continue;
            if (d == 4 && fours == 0) continue;

            unsigned twos_left  = twos  - (d == 2 ? 1 : 0);
            unsigned fours_left = fours - (d == 4 ? 1 : 0);

            seq_t comp = count_completions(rem, twos_left, fours_left);
            if (comp > k) {
                result = result * 5 + (seq_t)d;
                twos  = twos_left;
                fours = fours_left;
                break;
            } else {
                k -= comp;
            }
        }
    }

    return result;
}
