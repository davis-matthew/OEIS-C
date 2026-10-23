/*
 * A177960  Numbers of the form A001317(t), excluding those at places of the
 *          form t=m*(2^k-1), m>=0, k>=2.
 * https://oeis.org/A177960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of Fermat numbers (F_i = 2^(2^i)+1) where i runs
 *            over the * 1‑bits of the n‑th integer that is NOT divisible by
 *            any Mersenne number 2^k-1 (k≥2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h" /* for seq_pow */

/* Allowed indices t: all positive integers not divisible by any Mersenne number >=3. */
static const idx_t mersenne_nums[] = {
    3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047,
    4095, 8191, 16383, 32767, 65535
};
static const size_t MERSENNE_CNT = sizeof(mersenne_nums) / sizeof(mersenne_nums[0]);

/* Pre‑computed Fermat numbers F_i = 2^(2^i)+1 for i = 0..15. */
static seq_t fermat[16];
static bool fermat_inited = false;

static void init_fermat(void)
{
    if (fermat_inited) return;
    fermat_inited = true;
    for (int i = 0; i < 16; ++i) {
        idx_t exp = 1ULL << i;                /* 2^i */
        seq_t pow2 = seq_pow((seq_t)2, exp);   /* 2^(2^i) */
        fermat[i] = pow2 + (seq_t)1;          /* 2^(2^i)+1 */
    }
}

/* Return the n-th term (n≥1) of OEIS A177960. */
seq_t a(idx_t n)
{
    init_fermat();

    idx_t t = 1;            /* current integer being tested */
    idx_t count = 0;        /* how many valid integers we have seen */

    /* Find the n-th valid integer t_n. */
    while (true) {
        bool bad = false;
        for (size_t i = 0; i < MERSENNE_CNT; ++i) {
            if (t % mersenne_nums[i] == 0) { bad = true; break; }
        }
        if (!bad) {
            ++count;
            if (count == n) break; /* found t_n */
        }
        ++t;
    }

    /* Compute product of Fermat numbers whose bits are set in t. */
    seq_t result = (seq_t)1;
    idx_t mask = 1;
    int idx = 0;
    while (mask <= t) {
        if (t & mask) {
            result = result * fermat[idx];
        }
        ++idx;
        mask <<= 1;
    }
    return result;
}
