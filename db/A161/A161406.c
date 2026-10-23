/*
 * A161406  Write the Spanish name for n (ignoring spaces) and add numerical
 *          values of letters using a=1, b=2, ..., ñ=15, ..., y=26, z=27, n >=
 *          0.
 * https://oeis.org/A161406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = sum of Spanish letter values of the name of n (ignoring
 *            spaces), using a=1, b=2, …, ñ=15, …, y=26, z=27
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Letter value sums for the basic Spanish words used in the number names. */
static const seq_t sum_cero          = 43;

/* Units (0-9) - index 0 unused */
static const seq_t unit_sum[10] = {0, 52, 40, 65, 82, 45, 54, 60, 43, 69};

/* Tens (20,30,...,90) – indices 3..9; 0,1,2 unused */
static const seq_t tens_sum[10] = {0,0,0,90,86,92,86,87,68,94};

/* Special two‑digit numbers */
static const seq_t sum_diez          = 45;
static const seq_t sum_once          = 38;
static const seq_t sum_doce          = 28;
static const seq_t sum_trece         = 53;
static const seq_t sum_catorce       = 68;
static const seq_t sum_quince        = 71;
static const seq_t sum_dieciseis     = 84;
static const seq_t sum_diecisiete    = 90;
static const seq_t sum_dieciocho     = 73;
static const seq_t sum_diecinueve    = 99;
static const seq_t sum_veinte        = 77;
static const seq_t sum_veinti        = 81;

/* Hundreds */
static const seq_t sum_cien = 31;
static const seq_t sum_ciento = 68;
static const seq_t sum_cientos = 88;

/* High‑order words */
static const seq_t sum_mil      = 34;
static const seq_t sum_millon   = 76;
static const seq_t sum_millones = 101;

/* Hyphen / connector */
static const seq_t sum_y   = 26;

/* Special form “un” used before millones */
static const seq_t sum_un_million = 36;

/* ------------------------------------------------------------------------- */
/* Helper that adds the letter values for a two‑digit number 10‑99. */
static seq_t sum_two_digits(uint64_t m)
{
    if (m < 10)
        return unit_sum[(int)m];

    if (m <= 16) {
        switch ((int)m) {
            case 10: return sum_diez;
            case 11: return sum_once;
            case 12: return sum_doce;
            case 13: return sum_trece;
            case 14: return sum_catorce;
            case 15: return sum_quince;
            case 16: return sum_dieciseis;
        }
    }

    if (m <= 19) {
        switch ((int)m) {
            case 17: return sum_diecisiete;
            case 18: return sum_dieciocho;
            case 19: return sum_diecinueve;
        }
    }

    if (m == 20)
        return sum_veinte;

    if (m <= 29)            /* 21–29: veinti + unit */
        return sum_veinti + unit_sum[(int)(m - 20)];

    /* 30 and above – tens + “y” + unit when needed */
    uint64_t tens  = m / 10;
    uint64_t unit  = m % 10;
    seq_t sum = tens_sum[(int)tens];
    if (unit != 0)
        sum += sum_y + unit_sum[(int)unit];
    return sum;
}

/* ------------------------------------------------------------------------- */
/* Helper that adds the letter values for a 1–999 number. */
static seq_t sum_of_small(uint64_t m)
{
    seq_t sum = 0;
    uint64_t h   = m / 100;
    uint64_t rem = m % 100;

    if (h > 0) {
        if (h == 1) {                 /* 100–199 */
            if (rem == 0) {
                sum += sum_cien;       /* exactly 100 */
                return sum;
            }
            sum += sum_ciento;         /* 101–199 */
        } else {
            sum += unit_sum[(int)h] + sum_cientos;
        }
    }
    if (rem > 0)
        sum += sum_two_digits(rem);
    return sum;
}

/* ------------------------------------------------------------------------- */
/* Recursive helper that adds the letter values for any non‑negative number. */
static seq_t word_sum(idx_t n)
{
    seq_t total = 0;
    idx_t millions = n / 1000000ULL;
    n = n % 1000000ULL;

    if (millions > 0) {
        if (millions == 1) {                     /* “un millón” */
            total += sum_un_million + sum_millon;
        } else {                                 /* other millions */
            total += word_sum(millions) + sum_millones;
        }
    }

    idx_t thousands = n / 1000ULL;
    n = n % 1000ULL;

    if (thousands > 0) {
        if (thousands == 1) {                    /* “mil” */
            total += sum_mil;
        } else {                                 /* e.g. “doscientos mil” */
            total += sum_of_small(thousands) + sum_mil;
        }
    }

    if (n > 0)
        total += sum_of_small(n);

    return total;
}

/* ------------------------------------------------------------------------- */
/* Required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return sum_cero;
    return word_sum(n);
}
