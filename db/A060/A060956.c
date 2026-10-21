/*
 * A060956  Leading digit of 3^n.
 * https://oeis.org/A060956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Leading digit of 3^n
 * Formula: a(n) = floor(10^{frac(n*log10(3))})
 * (Frac denotes the fractional part of a real number.)
 *
 * This implementation uses integer arithmetic with a fixed‑point scaling
 * factor of 10^18.  The value of log10(3) is stored as an integer
 * multiple of that factor.  The leading digit is then obtained by
 * comparing the fractional part with pre–computed thresholds for each
 * possible first digit.
 */
#include <stdint.h>

static const uint64_t LOG_A_18 = 477121254719662437ULL;   /* floor(log10(3)*10^18) */
static const uint64_t SCALE_18 = 1000000000000000000ULL;  /* 10^18 */

static const uint64_t THRESHOLDS[10] = {
    0ULL,                          /* log10(1)   */
    301029995663981195ULL,         /* log10(2)   */
    477121254719662437ULL,         /* log10(3)   */
    602059991327962390ULL,         /* log10(4)   */
    698970004336018804ULL,         /* log10(5)   */
    778151250383643632ULL,         /* log10(6)   */
    845098040014256019ULL,         /* log10(7)   */
    903089986991943453ULL,         /* log10(8)   */
    954242509439324874ULL,         /* log10(9)   */
    1000000000000000000ULL         /* log10(10)  */
};

seq_t a(idx_t n)
{
    /* Compute (n * log10(3)) modulo 1, scaled by 10^18. */
    unsigned __int128 prod = (unsigned __int128) n * LOG_A_18;
    uint64_t rem = (uint64_t) (prod % SCALE_18);

    /* Determine the leading digit by comparing with thresholds. */
    for (int d = 1; d <= 9; ++d) {
        if (rem < THRESHOLDS[d]) {
            return (seq_t) d;
        }
    }
    /* In theory this line should never be reached because rem < 10^18. */
    return (seq_t) 9;
}
