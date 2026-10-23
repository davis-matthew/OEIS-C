/*
 * A026500  For n >= 3, a(n) = least positive integer > a(n-1) and not a(i)^2
 *          + a(j)^2 + a(k)^2 for 1<=i<=j<=k<=n.
 * https://oeis.org/A026500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 3, and for n >= 3, a(n) is the smallest integer
 *            * larger than a(n‑1) that is not expressible as * a(i)^2 +
 *            a(j)^2 + a(k)^2 with 1 <= i <= j <= k <= n. * This greedy
 *            construction yields the OEIS sequence A026500. * The code
 *            maintains a bitset of all sums of two squares of already...
 * Range:     u8 n <= 9, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>    /* for size_t */

#define BIT_CAP (200000000U)     /* 200 million bits (~25 MB) */

static const seq_t BIT_CAP_SEQ = (seq_t)BIT_CAP;

/* --- bitset helpers ----------------------------------------------------- */
static inline void set_bit(uint8_t *bits, size_t pos) {
    bits[pos >> 3] |= (uint8_t)(1U << (pos & 7U));
}
static inline uint8_t test_bit(const uint8_t *bits, size_t pos) {
    return (bits[pos >> 3] >> (pos & 7U)) & 1U;
}

/* --- global state ------------------------------------------------------- */
static uint8_t *pair_bits = NULL;     /* bitmap of pair sums           */
static seq_t *seq = NULL;             /* stored sequence values         */
static size_t seq_cap = 0;
static idx_t seq_len = 0;

/* --- dynamic array management ------------------------------------------ */
static void ensure_seq_cap(size_t need) {
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* --- resource initialization ------------------------------------------- */
static void init_resources(void) {
    if (pair_bits) return;
    size_t byte_cap = (BIT_CAP >> 3) + 1;
    pair_bits = (uint8_t *)calloc(byte_cap, sizeof(uint8_t));
    ensure_seq_cap(2);

    /* seed sequence with a(1)=1, a(2)=3 */
    seq[0] = (seq_t)1;
    seq[1] = (seq_t)3;
    seq_len = 2;

    /* store initial pair sums: 1^2+1^2, 1^2+3^2, 3^2+3^2 */
    set_bit(pair_bits, 2);     /* 1^2 + 1^2  */
    set_bit(pair_bits, 10);    /* 1^2 + 3^2  */
    set_bit(pair_bits, 18);    /* 3^2 + 3^2  */
}

/* --- compute next term in the sequence ------------------------------- */
static void compute_next(void) {
    /* candidate starts one larger than current last term */
    seq_t cand = seq[seq_len - 1] + 1;

    /* find first candidate that is not representable */
    while (1) {
        int rep = 0;    /* 1 if representable, 0 otherwise */
        for (size_t i = 0; i < seq_len; ++i) {
            seq_t sq  = seq[i] * seq[i];
            if (cand < sq) break;          /* further squares are larger */
            seq_t rem = cand - sq;          /* candidate - a(i)^2 */
            if (rem < BIT_CAP_SEQ) {
                size_t pos = (size_t)rem;   /* rem is guaranteed small */
                if (test_bit(pair_bits, pos)) { rep = 1; break; }
            }
        }
        if (!rep) break;   /* found unrepresentable candidate */
        cand = cand + 1;   /* next integer */
    }

    /* append found term */
    ensure_seq_cap(seq_len + 1);
    seq[seq_len] = cand;
    ++seq_len;

    /* update pair sum bitset with new element */
    seq_t T2 = cand * cand;               /* cand^2 */
    if ((seq_t)2 * T2 < BIT_CAP_SEQ) {    /* 2*cand^2 fits in bitset */
        set_bit(pair_bits, (size_t)((seq_t)2 * T2));
    }
    for (size_t j = 0; j < seq_len - 1; ++j) {
        seq_t sj  = seq[j] * seq[j];
        seq_t sum = T2 + sj;
        if (sum < BIT_CAP_SEQ) {
            set_bit(pair_bits, (size_t)sum);
        }
    }
}

/* --- public API -------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n <= 0 */
    init_resources();
    while (seq_len < n) compute_next();
    return seq[n - 1];
}
