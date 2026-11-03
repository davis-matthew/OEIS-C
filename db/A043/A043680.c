/*
 * A043680  Numbers whose base-16 representation has exactly 7 runs.
 * https://oeis.org/A043680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the number of runs in the base‑16 representation of x. */
static int hex_runs(seq_t x)
{
    if (x == 0)
        return 1;
    uint8_t prev = (uint8_t)(x & 0xFul);
    x >>= 4;
    int trans = 0;
    while (x) {
        uint8_t d = (uint8_t)(x & 0xFul);
        if (d != prev)
            ++trans;
        prev = d;
        x >>= 4;
    }
    return trans + 1;
}
/* This function enumerates integers in increasing order and returns the nth integer whose base‑16 representation has exactly 7 runs. */

seq_t a(idx_t n)
{
    /* The smallest number with 7 runs in hex is 0x1010101. */
    const seq_t start = (seq_t)0x1010101;
    seq_t cur = start;
    idx_t cnt = 0;

    for (;;) {
        if (hex_runs(cur) == 7) {
            ++cnt;
            if (cnt == n)
                return cur;
        }
        ++cur;
    }
}
