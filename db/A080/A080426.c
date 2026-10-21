/*
 * A080426  a(1)=1, a(2)=3; all terms are either 1 or 3; each run of 3's is
 *          followed by a run of two 1's; and a(n) is the length of the n-th
 *          run of 3's.
 * https://oeis.org/A080426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computes OEIS A080426 by iterating the morphism * 1 → 1,3,1 * 3
 *            → 1,3,3,3,1 * each run of 3's in the word thus generated has its
 *            length recorded. * a(n) is the length of the n-th run of 3's
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* buffer sizes – large enough for n up to about 10000 */
#define CUR_BUF_SIZE 2000000   /* 2 M bytes for current word */
#define RUN_LEN_CAP  20000     /* enough to hold a(n) up to n≈10000 */

/* morphisms as constant patterns */
static const unsigned char PAT1[3] = {1,3,1};
static const unsigned char PAT3[5] = {1,3,3,3,1};

/* workspace buffers */
static unsigned char cur_seq_buf[CUR_BUF_SIZE];
static unsigned char nxt_seq_buf[CUR_BUF_SIZE];
static unsigned char *cur_seq = cur_seq_buf;
static unsigned char *nxt_seq = nxt_seq_buf;
static size_t cur_len = 1;  /* length of current word */

/* buffers for run lengths */
static seq_t run_len[RUN_LEN_CAP];
static size_t run_cnt = 0;

/* state for counting runs */
static seq_t current_run = 0;
static bool in_run = false;

/* amount of of the current word that has already been processed
   when counting runs in successive expansions */
static size_t processed_len = 0;

/* ensure that run_len has at least n entries */
static void ensure_n (idx_t n)
{
    if (n == (idx_t)0) return;   /* out of range – nothing to do */

    /* initialise the starting word */
    cur_seq[0] = (unsigned char)1;
    cur_len   = 1;
    processed_len = 0;
    run_cnt   = 0;
    current_run = 0;
    in_run = false;

    while (run_cnt < (size_t)n) {
        size_t new_len = 0;

        /* expand the whole current word */
        for (size_t i = 0; i < cur_len; ++i) {
            unsigned char sym = cur_seq[i];
            const unsigned char *pat = (sym == (unsigned char)1) ? PAT1 : PAT3;
            size_t pat_len = (sym == (unsigned char)1) ? 3 : 5;

            for (size_t k = 0; k < pat_len; ++k) {
                unsigned char t = pat[k];
                nxt_seq[new_len++] = t;

                /* process only the suffix that has not been processed before */
                if (new_len - 1 >= processed_len) {
                    if (t == (unsigned char)3) {
                        current_run++;
                        in_run = true;
                    } else { /* t == 1 */
                        if (in_run) {
                            if (run_cnt < RUN_LEN_CAP) {
                                run_len[run_cnt++] = (seq_t)current_run;
                            }
                            current_run = 0;
                            in_run = false;
                        }
                    }
                }
            }
        }

        /* the generated word always ends with a 1, so no unfinished run remains */
        processed_len = new_len;

        /* swap buffers for next iteration */
        unsigned char *tmp = cur_seq;
        cur_seq = nxt_seq;
        nxt_seq = tmp;
        cur_len = new_len;
    }
}

/* public sequence function */
seq_t a (idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* out of range may return 0 */
    ensure_n (n);
    return run_len[(size_t)n-1];
}
