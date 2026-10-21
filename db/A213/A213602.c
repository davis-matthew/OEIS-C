/*
 * A213602  Numerator of expected minimum number of yes-no questions required
 *          to determine the value of a card randomly selected from a deck
 *          consisting of one 1, two 2's, three 3's, ..., and n n's.
 * https://oeis.org/A213602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the reduced fraction (sum of merge weights
 *            of the Huffman tree on weights 1,2,…,n) / ((n(n+1))/2). This is
 *            the numerator of the expected minimum number of yes–no questions
 *            needed to determine the value of a card drawn from a one‑to‑n
 *            deck where i appears i times
 * Range:     u8 n <= 11, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    size_t sz = (size_t)n;
    seq_t *h = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!h) return 0;    /* allocation failure – not expected */

    /* initialise heap with weights 1..n */
    for (size_t i = 0; i < sz; ++i) h[i] = (seq_t)(i + 1);

    size_t heap_size = sz;
    seq_t sum = 0;

    /* Huffman merging */
    while (heap_size > 1) {
        /* extract two minima */
        seq_t a = h[0];
        h[0] = h[--heap_size];
        size_t i = 0;
        while (1) {
            size_t left  = i * 2 + 1;
            size_t right = left + 1;
            size_t smallest = i;
            if (left  < heap_size && h[left]  < h[smallest]) smallest = left;
            if (right < heap_size && h[right] < h[smallest]) smallest = right;
            if (smallest == i) break;
            seq_t tmp = h[i]; h[i] = h[smallest]; h[smallest] = tmp;
            i = smallest;
        }

        seq_t b = h[0];
        h[0] = h[--heap_size];
        i = 0;
        while (1) {
            size_t left  = i * 2 + 1;
            size_t right = left + 1;
            size_t smallest = i;
            if (left  < heap_size && h[left]  < h[smallest]) smallest = left;
            if (right < heap_size && h[right] < h[smallest]) smallest = right;
            if (smallest == i) break;
            seq_t tmp = h[i]; h[i] = h[smallest]; h[smallest] = tmp;
            i = smallest;
        }

        seq_t merged = a + b;
        sum += merged;

        /* push merged weight back into heap */
        h[heap_size++] = merged;
        i = heap_size - 1;
        while (i > 0) {
            size_t parent = (i - 1) >> 1;
            if (h[parent] <= h[i]) break;
            seq_t tmp = h[i];
            h[i] = h[parent];
            h[parent] = tmp;
            i = parent;
        }
    }

    /* total number of cards */
    seq_t total = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t g = gcd(sum, total);
    seq_t numer = sum / g;

    free(h);
    return numer;
}
