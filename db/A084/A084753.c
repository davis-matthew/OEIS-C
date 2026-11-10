/*
 * A084753  Erroneous version of A081964.
 * https://oeis.org/A084753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n-th term of the triangle (row r holds r unused numbers
 *            coprime to r), built by search; guessed erroneous rule:
 *            multiples of 5 greater than 5 are skipped
 * Range:     i8 n <= 21, i16 n <= 21, i32 n <= 21, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t gcd_A084753(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t terms_cap = 0, terms_len = 0;
    static uint8_t *used = NULL;
    static size_t used_cap = 0;
    static uint64_t row = 0;
    static uint64_t cand = 1;

    while (terms_len < n) {
        row++;
        uint64_t selected = 0;
        cand = 1;
        while (selected < row) {
            if ((size_t)cand >= used_cap) {
                size_t nc = used_cap == 0 ? 256 : used_cap * 2;
                while (nc <= (size_t)cand) nc <<= 1;
                uint8_t *nu = (uint8_t *)malloc(nc);
                if (!nu) exit(1);
                memset(nu, 0, nc);
                if (used) { memcpy(nu, used, used_cap); free(used); }
                used = nu;
                used_cap = nc;
            }
            if (!used[cand] && !(cand % 5 == 0 && cand > 5) && gcd_A084753(cand, row) == 1) {
                used[cand] = 1;
                if (terms_len >= terms_cap) {
                    size_t nc = terms_cap == 0 ? 256 : terms_cap * 2;
                    uint64_t *nt = (uint64_t *)malloc(nc * sizeof(uint64_t));
                    if (!nt) exit(1);
                    if (terms) { memcpy(nt, terms, terms_len * sizeof(uint64_t)); free(terms); }
                    terms = nt;
                    terms_cap = nc;
                }
                terms[terms_len++] = cand;
                selected++;
            }
            cand++;
        }
    }
    return (seq_t)terms[n - 1];
}
