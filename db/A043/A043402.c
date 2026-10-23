/*
 * A043402  Numbers whose base-7 representation contains exactly two 2's.
 * https://oeis.org/A043402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-7 representation contains exactly two 2's.
   The function enumerates integers in order and counts the 2's in
   their base‑7 representations until the nth such number is found. */
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build system */
extern seq_t a(idx_t n);

/* helper: count how many 2's appear in the base‑7 representation of x */
static unsigned count_twos(idx_t x)
{
    unsigned cnt = 0;
    while (x > 0)
    {
        if (x % 7 == 2)
        {
            ++cnt;
            if (cnt > 2)
                return cnt;           /* early exit; value cannot qualify */
        }
        x /= 7;
    }
    return cnt;
}

/* external function implementing A043402 */
seq_t a(idx_t n)
{
    if (n == 0)                      /* undefined for the sequence offset */
        return (seq_t)0;             /* or some sentinel as the caller will
                                         never ask for n == 0 */

    idx_t count = 0;                  /* how many qualifying numbers seen */
    idx_t num   = 0;                  /* current candidate */

    while (true)
    {
        if (count_twos(num) == 2)
        {
            ++count;
            if (count == n)
                return (seq_t)num;    /* return the nth number */
        }
        ++num;
    }
}
