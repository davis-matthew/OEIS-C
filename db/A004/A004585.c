/*
 * A004585  Expansion of sqrt(10) in base 2.
 * https://oeis.org/A004585
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(10) * 2^(n-3)) mod 2, i.e. the binary digits
 *            of sqrt(10) = 11.00101001100...; computed exactly by the
 *            digit-by-digit (bitwise) integer square root on multi-limb
 *            integers
 * Range:     u8 n <= 9806, u16 n <= 9806, u32 n <= 9806, u64 n <= 9806,
 *            u128 n <= 9806, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *A004585_bits = NULL;
static size_t A004585_nb = 0, A004585_bcap = 0;
static uint32_t *A004585_Y = NULL, *A004585_R = NULL, *A004585_U = NULL;
static size_t A004585_cap = 0;
static size_t A004585_steps = 0;

static void A004585_ensure(size_t need) {
    if (need <= A004585_cap) return;
    size_t nc = A004585_cap ? A004585_cap : 8;
    while (nc < need) nc *= 2;
    A004585_Y = (uint32_t *)realloc(A004585_Y, nc * sizeof(uint32_t));
    A004585_R = (uint32_t *)realloc(A004585_R, nc * sizeof(uint32_t));
    A004585_U = (uint32_t *)realloc(A004585_U, nc * sizeof(uint32_t));
    memset(A004585_Y + A004585_cap, 0, (nc - A004585_cap) * sizeof(uint32_t));
    memset(A004585_R + A004585_cap, 0, (nc - A004585_cap) * sizeof(uint32_t));
    memset(A004585_U + A004585_cap, 0, (nc - A004585_cap) * sizeof(uint32_t));
    A004585_cap = nc;
}

static void A004585_push(unsigned char b) {
    if (A004585_nb == A004585_bcap) {
        A004585_bcap = A004585_bcap ? 2 * A004585_bcap : 64;
        A004585_bits = (unsigned char *)realloc(A004585_bits, A004585_bcap);
    }
    A004585_bits[A004585_nb++] = b;
}

static void A004585_extend(size_t want) {
    if (A004585_nb == 0) {
        A004585_ensure(8);
        A004585_Y[0] = 3; /* floor(sqrt(10)) */
        A004585_R[0] = 1; /* 10 - 3^2 */
        A004585_push(1);
        A004585_push(1);
    }
    while (A004585_nb < want) {
        A004585_steps++;
        size_t L = (A004585_steps + 8) / 32 + 2;
        A004585_ensure(L + 1);
        uint32_t *Y = A004585_Y, *R = A004585_R, *U = A004585_U;
        size_t i;
        /* R = 4R, U = 4Y + 1 */
        uint32_t cr = 0, cu = 0;
        for (i = 0; i < L; i++) {
            uint32_t r = R[i], y = Y[i];
            R[i] = (r << 2) | cr; cr = r >> 30;
            U[i] = (y << 2) | cu; cu = y >> 30;
        }
        U[0] |= 1;
        /* compare R >= U */
        int ge = 1;
        for (i = L; i-- > 0;) {
            if (R[i] != U[i]) { ge = R[i] > U[i]; break; }
        }
        unsigned char b = (unsigned char)ge;
        if (b) {
            uint64_t borrow = 0;
            for (i = 0; i < L; i++) {
                uint64_t d = (uint64_t)R[i] - U[i] - borrow;
                R[i] = (uint32_t)d;
                borrow = (d >> 63) & 1;
            }
        }
        /* Y = 2Y + b */
        uint32_t cy = b;
        for (i = 0; i < L; i++) {
            uint32_t y = Y[i];
            Y[i] = (y << 1) | cy; cy = y >> 31;
        }
        A004585_push(b);
    }
}

seq_t a(idx_t n) {
    size_t k = (size_t)(n - 2);
    A004585_extend(k + 1);
    return (seq_t)A004585_bits[k];
}
