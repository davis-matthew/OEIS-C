/*
 * A125937  Sprague-Grundy values for octal game .7737.
 * https://oeis.org/A125937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sprague–Grundy value for octal game .7737 (see A125937:
 *            .7 7 3 7 – 1‑token: 111; 2‑token: 111; 3‑token: 011; 4‑token:
 *            111; bits: 1 = remove all, 2 = leave a single heap, 4 = split
 *            into two heaps)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *sg = NULL;         /* cache of Grundy values */
static size_t sg_len = 0;        /* number of values currently stored (>=1) */

/* compute Grundy value for a single heap of size idx (idx >= 0) */
static void compute_sg(size_t idx)
{
    /* local bitmap of seen Grundy numbers – 256 is ample */
    static const size_t MAX_SEEN = 256;
    unsigned char seen[MAX_SEEN];
    memset(seen, 0, sizeof(seen));

    /* the octal digits for moves of 1..4 tokens */
    static const unsigned int codes[4] = {7, 7, 3, 7};
    static const unsigned int rems[4]  = {1, 2, 3, 4};

    for (int j = 0; j < 4; ++j) {
        size_t r = rems[j];
        if (r > idx) continue;
        unsigned int code = codes[j];

        /* move that removes all remaining tokens */
        if (code & 1) {
            seen[0] = 1;
        }

        /* move that leaves a single heap of size idx – r */
        if (code & 2) {
            seq_t val = sg[idx - r];
            unsigned long long hv = (unsigned long long)val;
            if (hv < MAX_SEEN) seen[(size_t)hv] = 1;
        }

        /* move that splits the remainder into two non‑empty heaps */
        if (code & 4) {
            size_t rem = idx - r;            /* tokens left after removal */
            if (rem >= 2) {
                for (size_t a = 1; a <= rem - 1; ++a) {
                    seq_t val = sg[a] ^ sg[rem - a];
                    unsigned long long hv = (unsigned long long)val;
                    if (hv < MAX_SEEN) seen[(size_t)hv] = 1;
                }
            }
        }
    }

    /* mex */
    seq_t mex = 0;
    for (size_t m = 0; m < MAX_SEEN; ++m) {
        if (!seen[m]) { mex = (seq_t)m; break; }
    }
    sg[idx] = mex;
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;           /* idx_t is unsigned 64‑bit */

    if (idx >= sg_len) {
        /* grow cache if necessary */
        size_t new_cap = sg_len > 0 ? sg_len << 1 : 1;
        while (new_cap <= idx) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(sg, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        sg = tmp;
        if (sg_len == 0) {
            sg[0] = 0;                /* Grundy of empty heap */
            sg_len = 1;
        }
        for (size_t i = sg_len; i <= idx; ++i) {
            compute_sg(i);
        }
        sg_len = idx + 1;
    }
    return sg[idx];
}
