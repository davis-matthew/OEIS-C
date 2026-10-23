/*
 * A146106  Bell numbers (A000110) read mod 16.
 * https://oeis.org/A146106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Bell(n) mod 16
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodicity: a(n+48) = a(n) for all n >= 0 */
/* Recurrence used for initialization: B(0)=1; B(n) = Σ_{k=0}^{n-1} C(n-1,k) * B(k) (mod 16) */

#include <stdbool.h>

static seq_t bell_mod16_cache[48];
static bool bell_mod16_inited = false;

static void bell_mod16_init(void)
{
    /* Basis value */
    bell_mod16_cache[0] = 1;

    /* Build Pascal triangle modulo 16 up to row 47 */
    unsigned char comb[48][48];
    comb[0][0] = 1;
    for (int i = 1; i < 48; ++i) {
        comb[i][0] = comb[i][i] = 1 & 0xF;
        for (int j = 1; j < i; ++j) {
            comb[i][j] = (comb[i - 1][j - 1] + comb[i - 1][j]) & 0xF;
        }
    }

    /* Compute Bell numbers modulo 16 */
    for (int n = 1; n < 48; ++n) {
        unsigned sum = 0;
        for (int k = 0; k < n; ++k) {
            sum += comb[n - 1][k] * (unsigned)bell_mod16_cache[k];
        }
        bell_mod16_cache[n] = (seq_t)(sum & 0xF);
    }

    bell_mod16_inited = true;
}

seq_t a(idx_t n)
{
    if (!bell_mod16_inited) {
        bell_mod16_init();
    }
    idx_t idx = n % 48;
    return bell_mod16_cache[idx];
}
