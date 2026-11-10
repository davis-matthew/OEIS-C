/*
 * A219643  Least inverse of A219642; a(n) = minimal i such that A219642(i) =
 *          n.
 * https://oeis.org/A219643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal i such that A219642(i) = n, where A219642(0)=0
 *            and A219642(i) = 1 + A219642(i - z(i)), z(i) = number of terms
 *            in the Zeckendorf expansion of i; computed by an incremental
 *            scan recording first occurrences
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9507 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static unsigned *A219643_f = 0;
static size_t A219643_fcap = 0;
static idx_t A219643_i = 0;          /* last index computed */
static idx_t *A219643_first = 0;
static size_t A219643_fcap2 = 0;
static size_t A219643_nfirst = 0;
static uint64_t A219643_fib[100];
static int A219643_nfib = 0;

static unsigned A219643_zc(uint64_t m)
{
    unsigned c = 0;
    int k = A219643_nfib - 1;
    while (m > 0) {
        while (A219643_fib[k] > m) k--;
        m -= A219643_fib[k];
        c++;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (A219643_nfib == 0) {
        A219643_fib[0] = 1;
        A219643_fib[1] = 2;
        int k = 2;
        while (A219643_fib[k - 1] <= UINT64_MAX - A219643_fib[k - 2] && k < 92) {
            A219643_fib[k] = A219643_fib[k - 1] + A219643_fib[k - 2];
            k++;
        }
        A219643_nfib = k;
        A219643_fcap = 1024;
        A219643_f = (unsigned *)malloc(A219643_fcap * sizeof(unsigned));
        A219643_fcap2 = 256;
        A219643_first = (idx_t *)malloc(A219643_fcap2 * sizeof(idx_t));
        A219643_f[0] = 0;
        A219643_first[0] = 0;
        A219643_nfirst = 1;
        A219643_i = 0;
    }
    while (A219643_nfirst <= (size_t)n) {
        idx_t i = A219643_i + 1;
        if ((size_t)i >= A219643_fcap) {
            A219643_fcap *= 2;
            A219643_f = (unsigned *)realloc(A219643_f, A219643_fcap * sizeof(unsigned));
        }
        unsigned z = A219643_zc((uint64_t)i);
        unsigned v = 1 + A219643_f[i - z];
        A219643_f[i] = v;
        A219643_i = i;
        if ((size_t)v == A219643_nfirst) {
            if (A219643_nfirst >= A219643_fcap2) {
                A219643_fcap2 *= 2;
                A219643_first = (idx_t *)realloc(A219643_first, A219643_fcap2 * sizeof(idx_t));
            }
            A219643_first[A219643_nfirst++] = i;
        }
    }
    return (seq_t)A219643_first[n];
}
