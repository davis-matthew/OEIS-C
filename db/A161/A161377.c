/*
 * A161377  Position of first letter of n in alphabet (in Spanish).
 * https://oeis.org/A161377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   spanish_letter_pos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: a(n)
   Computes the position of the first letter of the Spanish word for the
   decimal integer n.
   The Spanish alphabet is: ABCDEFGHIJKLMNÑOPQRSTUVWXYZ
   Positions: a=1, b=2, ..., n=14, ñ=15, o=16, …, z=27
*/
#include <stdint.h>
#include "../utils/spanish_letter_pos.h"

/* 0 -> Cero, 1..19 unique words */
static const char first_small[20] = {
    0,   /* unused for 0 */
    'u', /*  1 Uno */
    'd', /*  2 Dos */
    't', /*  3 Tres */
    'c', /*  4 Cuatro */
    'c', /*  5 Cinco */
    's', /*  6 Seis */
    's', /*  7 Siete */
    'o', /*  8 Ocho */
    'n', /*  9 Nueve */
    'd', /* 10 Diez */
    'o', /* 11 Once */
    'd', /* 12 Doce */
    't', /* 13 Trece */
    'c', /* 14 Catorce */
    'q', /* 15 Quince */
    'd', /* 16 Dieciséis */
    'd', /* 17 Diecisiete */
    'd', /* 18 Dieciocho */
    'd'  /* 19 Diecinueve */
};

/* first letter of the tens word for 20-99 (index by tens digit) */
static const char first_tens[10] = {
    0, 0,  /* 0-9 */
    'v',   /* 20-29 (veinte...) */
    't',   /* 30-39 (treinta...) */
    'c',   /* 40-49 (cuarenta...) */
    'c',   /* 50-59 (cincuenta...) */
    's',   /* 60-69 (sesenta...) */
    's',   /* 70-79 (setenta...) */
    'o',   /* 80-89 (ochenta...) */
    'n'    /* 90-99 (noventa...) */
};

/* first letter of the hundreds word for 100-999 (index by hundreds digit) */
static const char first_hundreds[10] = {
    0,          /* 0 */
    'c',        /* 100-199 (cien/ciento) */
    'd',        /* 200-299 (doscientos) */
    't',        /* 300-399 (trescientos) */
    'c',        /* 400-499 (cuatrocientos) */
    'q',        /* 500-599 (quinientos) */
    's',        /* 600-699 (seiscientos) */
    's',        /* 700-799 (setecientos) */
    'o',        /* 800-899 (ochocientos) */
    'n'         /* 900-999 (novecientos) */
};

/* Recursively compute the first letter of the Spanish word for n (n>0) */
static inline char first_letter(uint64_t n)
{
    if (n < 20)                 return first_small[n];
    if (n < 100)                return first_tens[n / 10];
    if (n < 1000)               return first_hundreds[n / 100];
    uint64_t k = n / 1000;      /* thousands group */
    if (k == 1)                 return 'm'; /* "mil" */
    return first_letter(k);     /* use the first letter of the thousands part */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return spanish_letter_pos('c');   /* "Cero" */
    char ch = first_letter((uint64_t)n);
    return spanish_letter_pos((unsigned char)ch);
}
