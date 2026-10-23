/*
 * A106686  Iterated expansion of the morphism 1->3, 2->1, 3->24, 4->6, 5->3,
 *          6->78, 7->8, 8->6.
 * https://oeis.org/A106686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth digit of the infinite word formed by iteratively
 *            concatenating blocks generated from the morphism 1→3, 2→1, 3→24,
 *            4→6, 5→3, 6→78, 7→8, 8→6. Block 0 is "1". Block 1 is image(Block
 *            0). For k≥1, Block k is Block k‑1 concatenated with image(Block
 *            k‑1). The sequence is the concatenatio...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static buffers that grow only as needed */
    static unsigned char *acc = NULL;      /* accumulated word */
    static size_t acc_len = 0, acc_cap = 0;
    static unsigned char *cur = NULL;      /* current block */
    static size_t cur_len = 0, cur_cap = 0;
    static int initialized = 0;

    /* first call – initialise */
    if (!initialized) {
        initialized = 1;
        acc_cap = 16;
        acc = (unsigned char *)malloc(acc_cap);
        cur_cap = 16;
        cur = (unsigned char *)malloc(cur_cap);

        /* Block 0 */
        cur[0] = '1';
        cur_len = 1;
        acc[0] = '1';
        acc_len = 1;
    }

    /* expand until we have enough characters */
    size_t idx = (size_t)n;
    while (acc_len <= idx) {
        /* compute image of current block */
        size_t img_cap = cur_len * 2 + 1;          /* ≤ twice the length */
        unsigned char *tmp = (unsigned char *)malloc(img_cap);
        size_t ti = 0;
        for (size_t i = 0; i < cur_len; ++i) {
            switch (cur[i]) {
                case '1': tmp[ti++] = '3';                break;
                case '2': tmp[ti++] = '1';                break;
                case '3': tmp[ti++] = '2'; tmp[ti++] = '4'; break;
                case '4': tmp[ti++] = '6';                break;
                case '5': tmp[ti++] = '3';                break;
                case '6': tmp[ti++] = '7'; tmp[ti++] = '8'; break;
                case '7': tmp[ti++] = '8';                break;
                case '8': tmp[ti++] = '6';                break;
            }
        }
        size_t img_len = ti;

        size_t new_len;
        if (cur_len == 1 && acc_len == 1) {
            /* first expansion – block 1 is just the image */
            new_len = img_len;
            if (cur_cap < new_len) {
                cur_cap = new_len;
                cur = (unsigned char *)realloc(cur, cur_cap);
            }
            memcpy(cur, tmp, new_len);
        } else {
            /* subsequent expansions – block k+1 = block k + image */
            new_len = cur_len + img_len;
            if (cur_cap < new_len) {
                cur_cap = new_len;
                cur = (unsigned char *)realloc(cur, cur_cap);
            }
            memcpy(cur + cur_len, tmp, img_len);
            cur_len = new_len;
            new_len = cur_len;       /* cur_len already updated */
        }
        free(tmp);

        /* append the new block to the accumulated word */
        cur_len = new_len;  /* update for first case as well */
        if (acc_len + cur_len > acc_cap) {
            acc_cap = acc_len + cur_len;
            acc = (unsigned char *)realloc(acc, acc_cap);
        }
        memcpy(acc + acc_len, cur, cur_len);
        acc_len += cur_len;
    }

    /* return the requested term (as integer 1‑8) */
    return (seq_t)(acc[idx] - '0');
}
