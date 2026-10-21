/*
 * A073611  Smallest m such that there are n primes of the form m-p where p is
 *          a prime. Index of the first occurrence of n in A073610.
 * https://oeis.org/A073611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 250, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A073611
   Formula: a(n) = smallest m such that there are n primes of the form m‑p,
            where p is prime.  Index of the first occurrence of n in
            A073610.  a(0) = 1, a(1) = 4, a(2) = 5 , ...
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

typedef struct { double re, im; } Complex;

// ---------- Fast Fourier Transform ----------
static void fft(Complex *a, size_t n, int invert)
{
    size_t j = 0;
    for (size_t i = 1; i < n; ++i) {
        size_t bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) {
            Complex tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
        }
    }

    for (size_t len = 2; len <= n; len <<= 1) {
        double ang = 2.0 * acos(-1.0) / (double)len * (invert ? -1.0 : 1.0);
        Complex wlen = { cos(ang), sin(ang) };
        for (size_t i = 0; i < n; i += len) {
            Complex w = { 1.0, 0.0 };
            for (size_t j = 0; j < len / 2; ++j) {
                Complex u = a[i + j];
                Complex v;
                v.re = w.re * a[i + j + len / 2].re - w.im * a[i + j + len / 2].im;
                v.im = w.re * a[i + j + len / 2].im + w.im * a[i + j + len / 2].re;

                a[i + j].re = u.re + v.re;
                a[i + j].im = u.im + v.im;
                a[i + j + len / 2].re = u.re - v.re;
                a[i + j + len / 2].im = u.im - v.im;

                // w = w * wlen
                double t = w.re * wlen.re - w.im * wlen.im;
                w.im = w.re * wlen.im + w.im * wlen.re;
                w.re = t;
            }
        }
    }

    if (invert) {
        for (size_t i = 0; i < n; ++i) {
            a[i].re /= (double)n;
            a[i].im /= (double)n;
        }
    }
}

// ---------- Convolution of prime indicator with itself ----------
static size_t compute_sequence(size_t limit, size_t cache_len, seq_t *cache, size_t target_n)
{
    // Sieve of Eratosthenes
    bool *isPrime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!isPrime) return 0;
    if (limit >= 2) {
        for (size_t i = 2; i <= limit; ++i) isPrime[i] = true;
        size_t sq = (size_t)sqrt((double)limit);
        for (size_t i = 2; i <= sq; ++i) {
            if (isPrime[i]) {
                for (size_t j = i * i; j <= limit; j += i)
                    isPrime[j] = false;
            }
        }
    }

    // Prepare FFT
    size_t N = 1;
    while (N <= 2 * limit) N <<= 1;
    Complex *A = (Complex *)calloc(N, sizeof(Complex));
    if (!A) { free(isPrime); return 0; }

    for (size_t i = 2; i <= limit; ++i)
        if (isPrime[i]) A[i].re = 1.0;

    fft(A, N, 0);
    // Square pointwise
    for (size_t i = 0; i < N; ++i) {
        double re = A[i].re, im = A[i].im;
        A[i].re = re * re - im * im;
        A[i].im = 2.0 * re * im;
    }
    fft(A, N, 1);

    size_t maxCount = 0;
    for (size_t m = 1; m <= limit; ++m) {
        size_t c = (size_t)(A[m].re + 0.5);
        if (c <= target_n && cache[c] == 0)
            cache[c] = (seq_t)m;
        if (c > maxCount) maxCount = c;
    }

    free(A);
    free(isPrime);
    return maxCount;
}

// ---------- Global cache ----------
static seq_t *cache = NULL;
static size_t cache_len = 0;

// ---------- Public function ----------
seq_t a(idx_t n)
{
    if (n >= cache_len) {                  // need more values
        size_t target = (size_t)n;
        size_t new_len = target + 1;
        seq_t *new_cache = (seq_t *)calloc(new_len, sizeof(seq_t));
        if (!new_cache) return 0;          // allocation failure

        // initial limit guess, roughly linear in n
        size_t limit = target * 200;
        if (limit < 128) limit = 128;

        while (1) {
            // recompute from scratch
            for (size_t i = 0; i < new_len; ++i) new_cache[i] = 0;

            size_t maxCount = compute_sequence(limit, new_len, new_cache, target);
            if (maxCount >= target) break;  // we have needed terms
            limit <<= 1;                    // increase range and try again
        }

        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    return cache[n];
}
