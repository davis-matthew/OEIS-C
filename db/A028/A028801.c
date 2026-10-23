/*
 * A028801  Nonsquares mod 88.
 * https://oeis.org/A028801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of positive integers that are not quadratic residues modulo 88 */

#include <stdbool.h>
#include <stddef.h>

static bool is_residue[88];
static seq_t prefix[89];
static bool tables_built = false;

/* Build tables of quadratic residues modulo 88 and prefix sums of non‑residues. */
static void build_tables(void) {
    /* Mark residues */
    for (idx_t i = 0; i < 88; ++i) {
        is_residue[i] = false;
    }
    for (idx_t i = 0; i < 88; ++i) {
        idx_t sq = (i * i) % 88;
        is_residue[sq] = true;
    }
    /* Prefix counts of non‑residues */
    prefix[0] = 0;
    for (idx_t i = 1; i <= 88; ++i) {
        seq_t add = is_residue[i - 1] ? 0 : 1;
        prefix[i] = prefix[i - 1] + add;
    }
    tables_built = true;
}

/* Count non‑residues <= m. */
static seq_t count_nonres(idx_t m) {
    if (!tables_built) build_tables();
    idx_t q = m / 88;
    idx_t r = m % 88;
    seq_t cnt = (seq_t)q * 70 + prefix[r + 1];
    return cnt;
}

/* Returns a(n): the n-th positive integer that is not a quadratic residue modulo 88. */
seq_t a(idx_t n) {
    if (!tables_built) build_tables();
    if (n == 0) return (seq_t)0;      /* safety for out‑of‑range call */

    idx_t low = 0;
    idx_t high = n;                    /* at least n */
    while (count_nonres(high) < n) {
        if (high > ((idx_t)-1) >> 1) {  /* avoid overflow */
            high = (idx_t)-1;
            break;
        }
        high <<= 1;                    /* double bound */
    }

    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (count_nonres(mid) >= n) {
            high = mid;
        } else {
            low = mid;
        }
    }
    return (seq_t)high;
}
