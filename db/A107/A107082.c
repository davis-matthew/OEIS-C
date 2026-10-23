/*
 * A107082  Put in lexicographic order and concatenate all sequences that
 *          start with 0 and have difference sequences that use the digits 1
 *          through 9 in order.
 * https://oeis.org/A107082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) is obtained by concatenating all 256 cumulative‑sum
 *            sequences that start with 0 and whose difference lists are
 *            formed from the digits 1..9 in order. For each mask (0..255) the
 *            string “123456789” is split at the bits of the mask; each block
 *            is interpreted as a decimal integer, and the cumulat...
 * Range:     u8 n <= 26, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

typedef struct {
    seq_t values[10];   /* 0 + 1..9 digits -> max 10 values (0 plus 9 sums) */
    size_t len;
} SubSeq;

/* Lexicographic comparison of two SubSeq objects. */
static int seqcmp(const void *a, const void *b) {
    const SubSeq *A = (const SubSeq *)a;
    const SubSeq *B = (const SubSeq *)b;
    size_t minlen = (A->len < B->len) ? A->len : B->len;
    for (size_t i = 0; i < minlen; ++i) {
        if (A->values[i] < B->values[i]) return -1;
        if (A->values[i] > B->values[i]) return 1;
    }
    if (A->len < B->len) return -1;
    if (A->len > B->len) return 1;
    return 0;
}

/* Compute all 256 sequences and flatten them into a single array. */
static void compute_terms(seq_t *out, size_t *total) {
    SubSeq seqs[256];
    size_t tot = 0;

    for (int mask = 0; mask < 256; ++mask) {
        seqs[mask].len = 0;
        seqs[mask].values[0] = 0;
        seqs[mask].len = 1;              /* the leading zero */

        seq_t cur = 0;
        seq_t val = 0;

        for (int d = 0; d <= 8; ++d) {
            val = val * 10 + (seq_t)(d + 1);   /* digit d+1 */
            if (mask & (1 << d)) {            /* cut after this digit */
                cur += val;
                seqs[mask].values[seqs[mask].len++] = cur;
                val = 0;
            }
        }
        /* final block after the last digit */
        cur += val;
        seqs[mask].values[seqs[mask].len++] = cur;

        tot += seqs[mask].len;
    }

    /* Sort lexicographically */
    qsort(seqs, 256, sizeof(SubSeq), seqcmp);

    /* Flatten into output array */
    size_t pos = 0;
    for (int i = 0; i < 256; ++i) {
        for (size_t j = 0; j < seqs[i].len; ++j) {
            out[pos++] = seqs[i].values[j];
        }
    }
    *total = tot;
}

seq_t a(idx_t n) {
    static int initialized = 0;
    static seq_t seq[1536];
    static size_t seq_len = 0;

    if (!initialized) {
        compute_terms(seq, &seq_len);
        initialized = 1;
    }

    if (n == 0 || n > seq_len)
        return 0;               /* out of range */
    return seq[n - 1];
}
