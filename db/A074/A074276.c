/*
 * A074276  Positions of occurrences of 2,1,1,2,1 in the Kolakoski sequence
 *          A000002.
 * https://oeis.org/A074276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 7265, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A074276: positions of the block 2,1,1,2,1 in the Kolakoski
 *  sequence.  This implementation builds the Kolakoski sequence
 *  incrementally and records the start indices of each occurrence.
 *  It returns the (n‑th) occurrence position.
 */

/* Dependencies required by the build environment */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Static state for incremental generation */
static uint8_t *kol_seq   = NULL;   /* array holding the Kolakoski sequence (1 or 2) */
static size_t   seq_cap   = 0;      /* capacity of kol_seq                  */
static size_t   seq_len   = 0;      /* number of terms currently stored     */
static idx_t    next_run  = 1;      /* next run index to process (0‑based)  */
static uint8_t  cur_sym   = 2;      /* symbol of the current run (1 or 2)   */

static idx_t *occ_pos = NULL;      /* array of positions (1‑based) of pattern */
static size_t  pos_cap = 0;        /* capacity of occ_pos */
static size_t  pos_cnt = 0;        /* number of occurrences stored */

/* Ensure capacity of the Kolakoski array */
static void grow_seq(void)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 64;
        uint8_t *tmp = (uint8_t*)realloc(kol_seq, new_cap * sizeof(uint8_t));
        if (!tmp) abort();
        kol_seq = tmp;
        seq_cap = new_cap;
    }
}

/* Ensure capacity of the positions array */
static void grow_pos(void)
{
    if (pos_cnt >= pos_cap) {
        size_t new_cap = pos_cap ? pos_cap << 1 : 64;
        idx_t *tmp = (idx_t*)realloc(occ_pos, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        occ_pos = tmp;
        pos_cap = new_cap;
    }
}

/* Initialise static structures on first call */
static void init(void)
{
    if (seq_len != 0) return;           /* already initialised */

    /* start Kolakoski with the first term 1 */
    seq_cap = 64;
    kol_seq = (uint8_t*)malloc(seq_cap * sizeof(uint8_t));
    if (!kol_seq) abort();
    kol_seq[0] = 1;
    seq_len = 1;

    next_run = 1;   /* next run to process (index of run length) */
    cur_sym  = 2;   /* symbol of the next run */

    occ_pos = NULL;
    pos_cap = 0;
    pos_cnt = 0;
}

/* Generate the Kolakoski sequence far enough to have at least n occurrences */
static void ensure_occurrences(idx_t n)
{
    init();

    while (pos_cnt < (size_t)n) {
        /* obtain the run length: if it has already been written,
           read it; otherwise it equals the current symbol (we are about
           to write it) */
        uint8_t run_len = (next_run < (idx_t)seq_len) ? kol_seq[next_run] : cur_sym;

        for (size_t k = 0; k < (size_t)run_len; ++k) {
            grow_seq();
            kol_seq[seq_len++] = cur_sym;

            /* check if the last five terms form the pattern 2,1,1,2,1 */
            if (seq_len >= 5) {
                size_t start = seq_len - 5;
                if (kol_seq[start]     == 2 &&
                    kol_seq[start + 1] == 1 &&
                    kol_seq[start + 2] == 1 &&
                    kol_seq[start + 3] == 2 &&
                    kol_seq[start + 4] == 1) {
                    grow_pos();
                    occ_pos[pos_cnt++] = (idx_t)(start + 1); /* 1‑based index */
                }
            }
        }

        /* advance to next run */
        ++next_run;
        cur_sym ^= 3;   /* toggle between 1 and 2 */
    }
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, safe return */

    ensure_occurrences(n);
    return (seq_t)occ_pos[n - 1];
}
