/*
 * A006159  a(n)=a(a(n-4))+a(n-a(n-4)).
 * https://oeis.org/A006159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(a(n-4)) + a(n - a(n-4)), a(1) = a(2) = a(3) = a(4) = 1
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *tab = NULL;
static idx_t tab_len = 0; /* tab[1..tab_len] computed */

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (n > tab_len) {
        idx_t newcap = n + n / 2 + 16;
        uint64_t *nt = (uint64_t *)realloc(tab, sizeof(uint64_t) * (newcap + 1));
        if (nt == NULL) return (seq_t)0;
        tab = nt;
        if (tab_len == 0) {
            tab[0] = 0;
            for (idx_t i = 1; i <= 4; i++) tab[i] = 1;
            tab_len = 4;
        }
        for (idx_t i = tab_len + 1; i <= newcap; i++) {
            uint64_t t = tab[i - 4];
            tab[i] = tab[t] + tab[i - t];
        }
        tab_len = newcap;
    }
    return (seq_t)tab[n];
}
