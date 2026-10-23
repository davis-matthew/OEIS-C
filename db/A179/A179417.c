/*
 * A179417  a(n) is the binary number (shown here in decimal) constructed from
 *          quadratic residues of 65537 in range [(n^2)+1,(n+1)^2] in such a
 *          way that quadratic residues are mapped to 1-bits, and
 *          non-quadratic residues (as well as the multiples of 65537) to
 *          0-bits, with the lower end of range mapped to les...
 * https://oeis.org/A179417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the binary number whose r‑th bit (0‑indexed from the *
 *            least‑significant end) is 1 iff (n²+1+r) is a quadratic residue
 *            of 65537 * (excluding multiples of 65537, which give 0 bits)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Quadratic residues modulo 65537 are pre‑computed once. */
#include <stdint.h>
#include <stdbool.h>

static bool residue[65537];
static bool residue_init = false;

static void init_residue(void)
{
    if (residue_init) return;
    for (int i = 0; i < 65537; ++i) residue[i] = false;
    for (uint64_t i = 0; i < 65537; ++i) {
        uint64_t sq = (i * i) % 65537ULL;
        residue[sq] = true;
    }
    residue_init = true;
}

/* Compute A179417(a(n)) */
seq_t a(idx_t n)
{
    init_residue();

    /* Compute the bounds of the interval [n^2+1, (n+1)^2] */
    uint64_t low  = (uint64_t)n * (uint64_t)n + 1ULL;        /* n^2 + 1      */
    uint64_t high = (uint64_t)(n + 1) * (uint64_t)(n + 1);  /* (n+1)^2      */
    uint64_t width = high - low + 1ULL;                     /* 2n + 1       */

    seq_t result = 0;
    seq_t pow    = 1;   /* 2^r for current bit position */

    for (uint64_t r = 0; r < width; ++r) {
        uint64_t x   = low + r;            /* current integer being tested */
        uint64_t rem = x % 65537ULL;       /* remainder modulo 65537 */

        if (rem != 0 && residue[rem]) {   /* residue and not a multiple of 65537 */
            result += pow;                /* set this bit */
        }
        pow <<= 1;                         /* move to next higher bit position */
    }

    return result;
}
