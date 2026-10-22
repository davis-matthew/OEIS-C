/*
 * A007938  Some nontrivial permutation of digits is a square.
 * https://oeis.org/A007938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search: k is a term if some arrangement of its digits other than
 *            k itself (leading zeros allowed) is a square, or k is a square
 *            with a repeated digit (swapping two equal digits counts as a
 *            nontrivial permutation)
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t isq(uint64_t x) {
    uint64_t r = 0, b = (uint64_t)1 << 32;
    while (b) {
        uint64_t t = r + b;
        if (t <= 0xFFFFFFFFULL && t * t <= x) r = t;
        b >>= 1;
    }
    return r;
}

static bool is_sq(uint64_t x) {
    uint64_t r = isq(x);
    return r * r == x;
}

static bool next_perm(int *arr, int len) {
    int i = len - 2;
    while (i >= 0 && arr[i] >= arr[i + 1]) i--;
    if (i < 0) return false;
    int j = len - 1;
    while (arr[j] <= arr[i]) j--;
    int t = arr[i]; arr[i] = arr[j]; arr[j] = t;
    for (int k = i + 1, l = len - 1; k < l; k++, l--) {
        t = arr[k]; arr[k] = arr[l]; arr[l] = t;
    }
    return true;
}

static bool is_term(uint64_t k) {
    int d[24];
    int len = 0;
    uint64_t m = k;
    while (m > 0) { d[len++] = (int)(m % 10); m /= 10; }
    if (len < 2) return false;
    for (int i = 0; i < len; i++)
        for (int j = i + 1; j < len; j++)
            if (d[i] > d[j]) { int t = d[i]; d[i] = d[j]; d[j] = t; }
    bool rep = false;
    for (int i = 0; i + 1 < len; i++) if (d[i] == d[i + 1]) rep = true;
    if (rep && is_sq(k)) return true;
    do {
        uint64_t v = 0;
        for (int i = 0; i < len; i++) v = v * 10 + (uint64_t)d[i];
        if (v != k && is_sq(v)) return true;
    } while (next_perm(d, len));
    return false;
}

static uint64_t *terms = NULL;
static idx_t nterms = 0, capterms = 0;
static uint64_t lastk = 0;

seq_t a(idx_t n) {
    while (nterms < n) {
        lastk++;
        if (is_term(lastk)) {
            if (nterms == capterms) {
                capterms = capterms ? capterms * 2 : 1024;
                terms = (uint64_t *)realloc(terms, (size_t)capterms * sizeof(uint64_t));
            }
            terms[nterms++] = lastk;
        }
    }
    return (seq_t)terms[n - 1];
}
