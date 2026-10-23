/*
 * A160373  Smallest number m such that exactly n triples (p,q,r) of distinct
 *          primes exist with m=p*q+r.
 * https://oeis.org/A160373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that exactly n triples (p,q,r) of
 *            distinct primes exist with m = p*q + r (p,q are unordered)
 * Range:     u8 n <= 23, u16 n <= 2490, u32 n <= 6567, u64 n <= 6567,
 *            u128 n <= 6567, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------------------------------------------------------------------- */
/*  Parameters (choose values large enough for n up to 10000)              */
/* ---------------------------------------------------------------------- */
static const idx_t LIMIT = 2000000;   /* search range for m                */
static const idx_t N_MAX  = 10000;    /* maximum n we pre‑compute           */

/* ---------------------------------------------------------------------- */
/*  Helper types                                                         */
/* ---------------------------------------------------------------------- */
typedef struct { double re; double im; } Complex;

/* ---------------------------------------------------------------------- */
/*  FFT (Cooley–Tukey) implementation                                  */
/* ---------------------------------------------------------------------- */
static void fft(Complex *a, size_t n, int invert)
{
    /* bit reversal */
    for (size_t i = 1, j = 0; i < n; ++i) {
        size_t bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) {
            Complex tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
        }
    }
    /* iterative FFT */
    for (size_t len = 2; len <= n; len <<= 1) {
        double ang = 2.0 * acos(-1.0) / len * (invert ? -1.0 : 1.0);
        double wlen_cos = cos(ang);
        double wlen_sin = sin(ang);
        for (size_t i = 0; i < n; i += len) {
            double wcos = 1.0, wsin = 0.0;
            for (size_t j = 0; j < len/2; ++j) {
                Complex u = a[i + j];
                Complex v;
                v.re = a[i + j + len/2].re * wcos
                     - a[i + j + len/2].im * wsin;
                v.im = a[i + j + len/2].re * wsin
                     + a[i + j + len/2].im * wcos;
                a[i + j].re = u.re + v.re;
                a[i + j].im = u.im + v.im;
                a[i + j + len/2].re = u.re - v.re;
                a[i + j + len/2].im = u.im - v.im;
                double next_wcos = wcos * wlen_cos - wsin * wlen_sin;
                double next_wsin = wcos * wlen_sin + wsin * wlen_cos;
                wcos = next_wcos;
                wsin = next_wsin;
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

/* ---------------------------------------------------------------------- */
/*  Simple sieve (uint32 primes)                                         */
/* ---------------------------------------------------------------------- */
static void simple_sieve(idx_t limit, uint32_t **primes_out, size_t *cnt_out)
{
    size_t n = (size_t)limit + 1;
    unsigned char *is_comp = (unsigned char *)calloc(n, 1);
    if (!is_comp) { *primes_out = NULL; *cnt_out = 0; return; }
    size_t primes_est = (size_t)(limit / log(limit) * 1.2) + 10;
    uint32_t *pr = (uint32_t *)malloc(primes_est * sizeof(uint32_t));
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (!is_comp[i]) {
            if (cnt + 1 > primes_est) {
                primes_est = primes_est * 3 / 2 + 10;
                uint32_t *tmp = (uint32_t *)realloc(pr, primes_est * sizeof(uint32_t));
                if (!tmp) { free(is_comp); free(pr); *primes_out = NULL; *cnt_out = 0; return; }
                pr = tmp;
            }
            pr[cnt++] = (uint32_t)i;
            if ((uint64_t)i * (uint64_t)i <= (uint64_t)limit)
                for (size_t j = i*i; j <= (size_t)limit; j += i) is_comp[j] = 1;
        }
    }
    free(is_comp);
    *primes_out = pr;
    *cnt_out = cnt;
}

/* ---------------------------------------------------------------------- */
/*  Global caches                                                        */
/* ---------------------------------------------------------------------- */
static seq_t *min_m_for_n = NULL;   /* size N_MAX+1, minimal m for each n   */
static bool   initialized = false;

/* ---------------------------------------------------------------------- */
/*  Initialization: build primes, compute convolution, fill cache.      */
/* ---------------------------------------------------------------------- */
static void init_seq(void)
{
    if (initialized) return;
    initialized = true;

    /* ----- Generate primes up to LIMIT -------------------------------- */
    uint32_t *primes = NULL;
    size_t prime_cnt = 0;
    simple_sieve(LIMIT, &primes, &prime_cnt);
    if (!primes || prime_cnt == 0) return;

    /* ----- Flag arrays for primes and for distinct prime products ------ */
    unsigned char *is_prime_flag = (unsigned char *)calloc((size_t)LIMIT + 1, 1);
    unsigned char *is_semiprime = (unsigned char *)calloc((size_t)LIMIT + 1, 1);
    if (!is_prime_flag || !is_semiprime) {
        free(primes);
        free(is_prime_flag); free(is_semiprime);
        return;
    }
    for (size_t i = 0; i < prime_cnt; ++i) {
        is_prime_flag[primes[i]] = 1;
    }
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        for (size_t j = i + 1; j < prime_cnt; ++j) {
            uint32_t q = primes[j];
            uint64_t prod = (uint64_t)p * (uint64_t)q;
            if (prod > (uint64_t)LIMIT) break;
            is_semiprime[(size_t)prod] = 1;
        }
    }

    /* ----- Prepare arrays for FFT ------------------------------------- */
    size_t nfft = 1;
    while (nfft <= 2 * (size_t)LIMIT) nfft <<= 1;
    Complex *A = (Complex *)calloc(nfft, sizeof(Complex));
    Complex *B = (Complex *)calloc(nfft, sizeof(Complex));
    if (!A || !B) {
        free(primes); free(is_prime_flag); free(is_semiprime);
        free(A); free(B);
        return;
    }
    for (size_t i = 0; i <= (size_t)LIMIT; ++i) {
        if (is_prime_flag[i]) A[i].re = 1.0;
        if (is_semiprime[i]) B[i].re = 1.0;
    }

    /* ----- FFT of both sequences ------------------------------------- */
    fft(A, nfft, 0);
    fft(B, nfft, 0);

    /* ----- Pointwise multiplication ----------------------------------- */
    for (size_t i = 0; i < nfft; ++i) {
        double real = A[i].re * B[i].re - A[i].im * B[i].im;
        double imag = A[i].re * B[i].im + A[i].im * B[i].re;
        A[i].re = real;
        A[i].im = imag;
    }

    /* ----- Inverse FFT ------------------------------------------------ */
    fft(A, nfft, 1);

    /* ----- Convert to integer counts --------------------------------- */
    size_t *cnt = (size_t *)calloc((size_t)LIMIT + 1, sizeof(size_t));
    if (!cnt) {
        free(primes); free(is_prime_flag); free(is_semiprime);
        free(A); free(B);
        return;
    }
    for (size_t i = 0; i <= (size_t)LIMIT; ++i) {
        cnt[i] = (size_t)(A[i].re + 0.5);   /* round to nearest integer */
    }

    /* ----- Subtract triples where r equals p or q --------------------- */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        for (size_t j = i + 1; j < prime_cnt; ++j) {
            uint32_t q = primes[j];
            uint64_t prod = (uint64_t)p * (uint64_t)q;
            if (prod > (uint64_t)LIMIT) break;
            uint64_t m1 = prod + p;
            uint64_t m2 = prod + q;
            if (m1 <= (uint64_t)LIMIT && cnt[m1] > 0) --cnt[m1];
            if (m2 <= (uint64_t)LIMIT && cnt[m2] > 0) --cnt[m2];
        }
    }

    /* ----- Allocate cache for minimal m per count -------------------- */
    min_m_for_n = (seq_t *)calloc((size_t)N_MAX + 1, sizeof(seq_t));
    if (!min_m_for_n) {
        free(primes); free(is_prime_flag); free(is_semiprime);
        free(A); free(B); free(cnt);
        return;
    }
    for (size_t m = 1; m <= (size_t)LIMIT; ++m) {
        size_t c = cnt[m];
        if (c <= (size_t)N_MAX && min_m_for_n[c] == 0)
            min_m_for_n[c] = (seq_t)m;
    }
    if (min_m_for_n[0] == 0) min_m_for_n[0] = (seq_t)1;

    /* ----- Free temporaries ------------------------------------------ */
    free(primes);
    free(is_prime_flag);
    free(is_semiprime);
    free(A);
    free(B);
    free(cnt);
}

/* ---------------------------------------------------------------------- */
/*  Public API: sequence a(idx_t n)                                       */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_seq();
    if (n <= N_MAX) return min_m_for_n[(size_t)n];
    return (seq_t)0;
}
