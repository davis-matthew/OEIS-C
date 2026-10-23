/*
 * A154818  Numbers k such that k^4 contains every digit exactly twice.
 * https://oeis.org/A154818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, fini, full
 * Formula:   Brute‑force search for k such that k^4 contains each digit 0–9
 *            exactly twice
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence has only four known terms. The code generates them on first use. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t *table = NULL;
    static size_t size   = 0;
    static size_t cap    = 0;

    /* Generate the table once */
    if (!table) {
        size = 0; cap = 4;
        table = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!table) return 0;  /* allocation failure, return zero */

        const idx_t LIMIT = 200000;  /* sufficient to cover all known solutions */
        for (idx_t k = 1; k <= LIMIT; ++k) {
            /* compute k^4 in 128‑bit to avoid overflow */
            unsigned __int128 val = (unsigned __int128)k * k;
            val = val * val;          /* now val = k^4 */

            /* count decimal digits */
            unsigned char cnt[10] = {0};
            unsigned __int128 t = val;
            if (t == 0) cnt[0] = 1;   /* not relevant here */
            while (t > 0) {
                int d = (int)(t % 10);
                cnt[d]++;
                t /= 10;
            }

            /* check that each digit occurs exactly twice */
            bool ok = true;
            for (int d = 0; d < 10; ++d) {
                if (cnt[d] != 2) { ok = false; break; }
            }
            if (ok) {
                if (size >= cap) {
                    cap <<= 1;
                    table = (seq_t *)realloc(table, cap * sizeof(seq_t));
                    if (!table) return 0; /* allocation failure */
                }
                table[size++] = (seq_t)k;
            }
        }
    }

    /* Return the requested term (indexing starts at 1) */
    if (n == 0 || n > size) return 0;
    return table[n - 1];
}
