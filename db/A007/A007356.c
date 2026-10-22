/*
 * A007356  Apocalyptic powers: 2^a(n) contains 666.
 * https://oeis.org/A007356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   search: a(n) = n-th k such that the decimal digits of 2^k
 *            contain "666"; 2^k is kept as a decimal digit array and doubled
 *            step by step
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static unsigned char *dig = NULL;   /* little-endian decimal digits of 2^cur_k */
static size_t nd = 0, dcap = 0;
static uint64_t cur_k = 0;
static uint64_t *terms = NULL;
static size_t nterms = 0, tcap = 0;

static void double_digits(void) {
    unsigned carry = 0;
    size_t i;
    for (i = 0; i < nd; i++) {
        unsigned v = (unsigned)dig[i] * 2u + carry;
        if (v >= 10) { dig[i] = (unsigned char)(v - 10); carry = 1; }
        else { dig[i] = (unsigned char)v; carry = 0; }
    }
    if (carry) {
        if (nd == dcap) {
            dcap = dcap ? dcap * 2 : 64;
            dig = (unsigned char *)realloc(dig, dcap);
        }
        dig[nd++] = (unsigned char)carry;
    }
}

static bool has_666(void) {
    int run = 0;
    size_t i;
    for (i = 0; i < nd; i++) {
        if (dig[i] == 6) {
            if (++run == 3) return true;
        } else {
            run = 0;
        }
    }
    return false;
}

seq_t a(idx_t n) {
    if (dig == NULL) {
        dcap = 64;
        dig = (unsigned char *)malloc(dcap);
        dig[0] = 1;
        nd = 1;
        cur_k = 0;
    }
    while (nterms < n) {
        double_digits();
        cur_k++;
        if (has_666()) {
            if (nterms == tcap) {
                tcap = tcap ? tcap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, tcap * sizeof(uint64_t));
            }
            terms[nterms++] = cur_k;
        }
    }
    return (seq_t)terms[n - 1];
}
