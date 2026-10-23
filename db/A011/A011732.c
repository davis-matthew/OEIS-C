/*
 * A011732  A binary m-sequence: expansion of reciprocal of x^19 + x^6 + x^5 +
 *          x + 1 (mod 2, shifted by 18 initial 0's).
 * https://oeis.org/A011732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth term of the binary m‑sequence obtained by
 *            expanding the reciprocal of x^19 + x^6 + x^5 + x + 1 over GF(2),
 *            then shifted right by 18 zeros
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static const idx_t PERIOD = ((idx_t)1 << 19) - 1;
static uint8_t *seq_arr = NULL;
static bool seq_init = false;

static void build_seq(void) {
    seq_arr = (uint8_t *)malloc(PERIOD * sizeof(uint8_t));
    if (!seq_arr) return; /* allocation failure – allowed to return 0 later */

    seq_arr[0] = 1;                      /* a(0) of the unshifted series */
    for (idx_t i = 1; i < PERIOD; ++i) {
        uint8_t a1  = (i >= 1)  ? seq_arr[i-1]  : 0;
        uint8_t a5  = (i >= 5)  ? seq_arr[i-5]  : 0;
        uint8_t a6  = (i >= 6)  ? seq_arr[i-6]  : 0;
        uint8_t a19 = (i >= 19) ? seq_arr[i-19] : 0;
        seq_arr[i] = a1 ^ a5 ^ a6 ^ a19;  /* recurrence over GF(2) */
    }
    seq_init = true;
}

seq_t a(idx_t n) {
    if (!seq_init) build_seq();
    if (n < 18) return (seq_t)0;            /* shifted by 18 leading zeros */
    idx_t k = (n - 18) % PERIOD;
    return (seq_t)seq_arr[k];
}
