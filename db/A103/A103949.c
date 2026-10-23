/*
 * A103949  a(n) is the number of distinct n-th powers of functions {1, 2, 3,
 *          4} -> {1, 2, 3, 4}.
 * https://oeis.org/A103949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of distinct n-th powers of all functions mapping
 *            {1,2,3,4} -> {1,2,3,4}
 * Range:     u8 n <= 0, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool init = false;
static uint8_t  comp[256][256];
static uint8_t  decoded[256][4];
static const uint8_t ID = 228; /* identity map: 0,1,2,3 in base‑4 */

static void init_tables(void)
{
    if (init) return;
    /* decode each index into its 4 outputs (0..3) */
    for (uint16_t i = 0; i < 256; ++i) {
        uint16_t v = i;
        for (int k = 0; k < 4; ++k) {
            decoded[i][k] = v & 3;
            v >>= 2;
        }
    }
    /* compose table: comp[a][b] = a ∘ b */
    for (uint16_t a = 0; a < 256; ++a) {
        for (uint16_t b = 0; b < 256; ++b) {
            uint8_t res[4];
            for (int x = 0; x < 4; ++x) {
                res[x] = decoded[a][ decoded[b][x] ];
            }
            /* encode back to index */
            uint8_t idx = 0;
            for (int k = 3; k >= 0; --k) {
                idx = (idx << 2) | res[k];
            }
            comp[a][b] = idx;
        }
    }
    init = true;
}

/* exponentiation of a function index by n (binary exponentiation) */
static inline uint8_t fn_pow(uint8_t f, idx_t n)
{
    if (n == 0) return ID;
    uint8_t res = ID;
    uint8_t base = f;
    while (n) {
        if (n & 1) res = comp[res][base];
        base = comp[base][base];
        n >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    init_tables();
    bool seen[256] = {0};
    seq_t count = 0;
    for (uint16_t f = 0; f < 256; ++f) {
        uint8_t g = fn_pow((uint8_t)f, n);
        if (!seen[g]) {
            seen[g] = true;
            ++count;
        }
    }
    return count;
}
