/*
 * A008537  Numbers that do not contain the letter 'n'.
 * https://oeis.org/A008537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses base 43 representation over allowed two‑digit blocks with
 * weights 10^(9*i).  a(1)=0, and for n≥2 let n−2 be written in base 43
 * with digits d0…d(k−1) (least significant first).  The most
 * significant digit d(k−1) is increased by one to avoid the value 0.
 * Then a(n)=∑_{i=0}^{k−1} S[di]·10^(9*i), where S is the sorted
 * list of integers <100 whose English words contain no “n”.
 * This enumerates all numbers whose English representation contains no
 * letter “n”. */
#include <stddef.h>
#include <stdint.h>

static const seq_t S[] = {
    0, 2, 3, 4, 5, 6, 8, 12, 30, 32, 33, 34, 35, 36, 38, 40,
    42, 43, 44, 45, 46, 48, 50, 52, 53, 54, 55, 56, 58, 60,
    62, 63, 64, 65, 66, 68, 80, 82, 83, 84, 85, 86, 88
};

seq_t a(idx_t n)
{
    if (n == 1)          /* a(1) = 0 */
        return 0;

    const idx_t B = sizeof S / sizeof *S;        /* 43 */
    const idx_t Bm1 = B - 1;                     /* 42 */

    idx_t idx = n - 2;    /* 0‑based index among non‑zero terms */

    /* Find block length k */
    idx_t k = 1;
    unsigned __int128 powB = 1;        /* B^(k-1) */
    while (1) {
        unsigned __int128 cnt = (unsigned __int128)Bm1 * powB;
        if (idx < (idx_t)cnt)
            break;
        idx -= (idx_t)cnt;
        k++;
        powB *= B;
    }

    /* Extract base‑B digits, least significant first */
    idx_t raw[64] = {0};
    idx_t rem = idx;
    for (idx_t i = 0; i < k; i++) {
        raw[i] = rem % B;
        rem /= B;
    }
    raw[k - 1]++;   /* make most significant digit >0 */

    /* Reconstruct the numeric value */
    seq_t result = 0;
    const seq_t pow10_9 = (seq_t)1000000000ULL;
    for (idx_t i = k; i-- > 0;) {          /* i = k-1 .. 0 */
        result = result * pow10_9 + S[(size_t)raw[i]];
    }
    return result;
}
