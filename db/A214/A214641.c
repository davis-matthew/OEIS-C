/*
 * A214641  Indices of a in the sequence closed under a -> abc, b -> ab, c ->
 *          b . Start with a.
 * https://oeis.org/A214641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Indices of 'a' in the fixed point of the morphism
   a → abc, b → ab, c → b.
   The function builds the word level by level,
   counting the positions of 'a' until the required n‐th one is found. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)          /* sequence is 1‑based; n≥1 is always asked */
        return 0;

    /* Two buffers that alternate as current and next word */
    uint8_t *buf[2];
    size_t len[2] = {0, 0};
    size_t cap[2] = {8, 8};

    buf[0] = (uint8_t *)malloc(cap[0] * sizeof(uint8_t));
    if (!buf[0]) return 0;
    buf[1] = (uint8_t *)malloc(cap[1] * sizeof(uint8_t));
    if (!buf[1]) { free(buf[0]); return 0; }

    /* Start with the single letter 'a' */
    buf[0][0] = 0;               /* 0 = 'a', 1 = 'b', 2 = 'c' */
    len[0] = 1;
    int cur = 0;                 /* index of the buffer containing current word */
    int nxt = 1;                 /* index of the buffer to write the next word */

    while (1) {
        /* Count the a's in the current word */
        idx_t count_a = 0;
        for (size_t i = 0; i < len[cur]; ++i) {
            if (buf[cur][i] == 0) {          /* letter 'a' */
                ++count_a;
                if (count_a == n) {
                    /* Found the n-th 'a'; clean up and return its index (1‑based) */
                    free(buf[0]); free(buf[1]);
                    return (seq_t)(i + 1);
                }
            }
        }

        /* If we haven't found it yet, generate the next word */
        size_t new_len = 0;     /* length of the word to be written in buf[nxt] */
        for (size_t i = 0; i < len[cur]; ++i) {
            uint8_t ch = buf[cur][i];

            /* ensure capacity for the new word, adding up to 3 letters per step */
            while (new_len + 3 > cap[nxt]) {
                cap[nxt] <<= 1;
                uint8_t *tmp = (uint8_t *)realloc(buf[nxt], cap[nxt] * sizeof(uint8_t));
                if (!tmp) { free(buf[0]); free(buf[1]); return 0; }
                buf[nxt] = tmp;
            }

            /* Apply the morphism */
            if (ch == 0) {                   /* 'a' → abc */
                buf[nxt][new_len++] = 0;     /* a */
                buf[nxt][new_len++] = 1;     /* b */
                buf[nxt][new_len++] = 2;     /* c */
            } else if (ch == 1) {            /* 'b' → ab */
                buf[nxt][new_len++] = 0;     /* a */
                buf[nxt][new_len++] = 1;     /* b */
            } else {                         /* 'c' → b */
                buf[nxt][new_len++] = 1;     /* b */
            }
        }

        len[nxt] = new_len;

        /* Prepare for the next iteration */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Should never reach here */
    free(buf[0]); free(buf[1]);
    return 0;
}
